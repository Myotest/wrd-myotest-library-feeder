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

#include "wrd-myotest-library-feeder/MyotestLibraryFeeder.h"

MyotestLibraryFeeder::MyotestLibraryFeeder()
{
	init();
}
void MyotestLibraryFeeder::init()
{	
	myoAPI_init(ACTIVITY_MONITOR);
	index = 0;
	start();
}

void MyotestLibraryFeeder::start()
{
	myoAPI_acquisition(0,0,0,0);
	myoAPI_acquisition(0,0,0,0);
	//minar::Scheduler::postCallback(this,&MyotestLibraryFeeder::getAcceleration);
}

void MyotestLibraryFeeder::stop()
{
	
}

void MyotestLibraryFeeder::getAcceleration()
{
	myoAPI_acquisition(0,0,0,0);
	// if(index < rawdata1.length)
	// {
		// sample_t currentSample = rawdata1.data[index];
		// datacollection_convertTiTag2Mg(&currentSample);
		// myoAPI_acquisition(currentSample.timestamp, currentSample.x, currentSample.y, currentSample.z);
		
		// index++;
	// }
}