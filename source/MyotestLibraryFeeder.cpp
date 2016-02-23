/* **************************************************************************
*
*
     __  __             _            _      _____
    |  \/  |           | |          | |    / ____|    /\
    | \  / |_   _  ___ | |_ ___  ___| |_  | (___     /  \
    | |\/| | | | |/ _ \| __/ _ \/ __| __|  \___ \   / /\ \
    | |  | | |_| | (_) | ||  __/\__ \ |_   ____) | / ____ \ _
    |_|  |_|\__, |\___/ \__\___||___/\__| |_____(_)_/    \_(_)
             __/ |
            |___/
*
* Copyright (c) 2004-2016 Myotest SA
*
* PackageLicenseDeclared: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
****************************************************************************/

#include "wrd-myotest-library-feeder/MyotestLibraryFeeder.h"

/*****************************************************************************/
/*****************************************************************************/

#include "wrd-sensor-accelerometer/BMA2X2.h"
#include "wrd-sensor-gyroscope/BMG160.h"
#include "gpio-switch/DigitalOutEx.h"
#include "gpio-switch/InterruptInEx.h"

static DigitalOutEx gpower(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_GYROSCOPE_ENABLE_PIN,
                           YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_GYROSCOPE_ENABLE_LOCATION);

static InterruptInEx girq(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_GYROSCOPE_IRQ_PIN,
                          YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_GYROSCOPE_IRQ_LOCATION);

static BMG160 gyro(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_GYROSCOPE_I2C_NAME,
                   gpower, girq);


static DigitalOutEx apower(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_ACCELEROMETER_ENABLE_PIN,
                           YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_ACCELEROMETER_ENABLE_LOCATION);

static InterruptInEx airq(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_ACCELEROMETER_IRQ_PIN,
                          YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_ACCELEROMETER_IRQ_LOCATION);

static BMA2X2 accel(YOTTA_CFG_HARDWARE_WEARABLE_REFERENCE_DESIGN_SENSOR_ACCELEROMETER_I2C_NAME,
                    apower, airq);

static uint8_t rawBuffer[6];

static void getRawBuffer();

static void getRawBufferDone(uint8_t count)
{
    /* Reschedule next data dump */
    minar::Scheduler::postCallback(getRawBuffer)
        .delay(minar::milliseconds(10))
        .tolerance(1);

    if (count > 0)
    {
        BMA2X2::acceleration_t measurement = { 0, 0, 0, 0};
        accel.getSampleFromBuffer(rawBuffer, 0, measurement);

        myoAPI_acquisition(minar::ticks(minar::platform::getTime()),
                           measurement.x,
                           measurement.y,
                           measurement.z);

        myoAPI_process();
    }
}

static void getRawBuffer()
{
    /* get one frame (6 bytes) */
    accel.getRawBuffer(rawBuffer, 6, getRawBufferDone);
}

static void accelReady()
{
    minar::Scheduler::postCallback(getRawBuffer)
        .delay(minar::milliseconds(10))
        .tolerance(1);
}

static void setFifo()
{
    /* set FIFO to only contain 1 frame */
    accel.setFifo(BMA2X2::FIFO_BYPASS, accelReady);
}

static void setBandwidth()
{
    accel.setBandwidth(BMA2X2::BANDWIDTH_125HZ, setFifo);
}

static void setRange()
{
    gyro.sleep();
    accel.setRange(BMA2X2::RANGE_2G, setBandwidth);
}

static void startAccelerometer()
{
    accel.powerOn(setRange);
}

/*****************************************************************************/
/*****************************************************************************/

MyotestLibraryFeeder* MyotestLibraryFeeder::m_instance = NULL;
MyotestLibraryFeeder* MyotestLibraryFeeder::Instance()
{
    if(!m_instance)
    {
        m_instance = new MyotestLibraryFeeder();
        m_instance->init();
    }
    return m_instance;
}
MyotestLibraryFeeder::~MyotestLibraryFeeder()
{
    myoAPI_release();
}

void MyotestLibraryFeeder::init()
{
    myoAPI_init(ACTIVITY_MONITOR);
    index = 0;
    countersReseted = false;
}

void MyotestLibraryFeeder::start()
{
#if 1
    startAccelerometer();
#else
    minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::getAcceleration).delay(minar::milliseconds(10));
#endif
    minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::resetCounters).delay(minar::milliseconds(10000));
}

void MyotestLibraryFeeder::getAcceleration()
{
    if(index < rawdata2.length)
    {
        sample_t currentSample = rawdata2.data[index];
        datacollection_convertTiTag2Mg(&currentSample);
        myoAPI_acquisition(currentSample.timestamp, currentSample.x, currentSample.y, currentSample.z);
        myoAPI_process();

        index++;

        minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::getAcceleration).delay(minar::milliseconds(10));
    }
}

void MyotestLibraryFeeder::resetCounters()
{
    time_t seconds = time(NULL);

    struct tm *tm_struct = localtime(&seconds);
    int hour = tm_struct->tm_hour;

    if(hour == 0 && countersReseted == false)
    {
        myoAPI_resetCounters();
        countersReseted = true;
    }
    else if(hour > 0)
    {
        countersReseted = false;
    }

    minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::resetCounters).delay(minar::milliseconds(10000));
}