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
    //myoAPI_release();
}

void MyotestLibraryFeeder::init()
{
    myoAPI_init(ACTIVITY_MONITOR);
    index = 0;
    countersReseted = false;
}

void MyotestLibraryFeeder::start()
{
    minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::getAcceleration).delay(minar::milliseconds(10));
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