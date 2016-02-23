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

#ifndef __WRD_MYOTEST_LIBRARY_FEEDER_H__
#define __WRD_MYOTEST_LIBRARY_FEEDER_H__

#include "mbed-drivers/mbed.h"
#include "time.h"
#include "sys/time.h"

#include "myotest-library-step-analysis/wrd_public.h"
#include "myotest-prerecorded-data-collection/datacollection.h"
#include "myotest-step-analysis-data-set-1/dataset.h"
#include "myotest-step-analysis-data-set-2/dataset.h"

class MyotestLibraryFeeder
{
public:
    static MyotestLibraryFeeder* Instance();
    ~MyotestLibraryFeeder();

    void start();

private:
    static MyotestLibraryFeeder *m_instance;
    uint32_t index;
    bool countersReseted;

    MyotestLibraryFeeder(){};
    void init();
    void getAcceleration();
    void resetCounters();

};
#endif // __WRD_MYOTEST_LIBRARY_FEEDER_H__
