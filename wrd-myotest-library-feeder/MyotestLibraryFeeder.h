/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __WRD_MYOTEST_LIBRARY_FEEDER_H__
#define __WRD_MYOTEST_LIBRARY_FEEDER_H__

#include "mbed-drivers/mbed.h"

#include "myotest-lib/public.h"
#include "myotest-data-collection/datacollection.h"
#include "myotest-data-set-1/dataset.h"
#include "myotest-data-set-2/dataset.h"

class MyotestLibraryFeeder
{
public:
    /**
     * @brief MyotestLibraryFeeder constructor.
     */
    MyotestLibraryFeeder();
	void init();
	void start();
	void stop();


private:
   void getAcceleration();
   uint32_t index;
};
#endif // __WRD_MYOTEST_LIBRARY_FEEDER_H__
