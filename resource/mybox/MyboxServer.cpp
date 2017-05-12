//******************************************************************
//
// Copyright 2014 Intel Mobile Communications GmbH All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

///
/// This sample provides steps to define an interface for a resource
/// (properties and methods) and host this resource on the server.
///
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif
#include <mutex>
#include <condition_variable>

#include "OCPlatform.h"
#include "OCApi.h"
#include "ocpayload.h"

#include "MyboxResource.h"
#include "MyboxCfg.h"

using namespace OC;
using namespace std;
namespace PH = std::placeholders;

static const char* SVR_DB_FILE_NAME = "./oic_svr_db_server.dat";


// OCPlatformInfo Contains all the platform info to be stored
OCPlatformInfo platformInfo;


void DeletePlatformInfo() {
	delete[] platformInfo.platformID;
	delete[] platformInfo.manufacturerName;
	delete[] platformInfo.manufacturerUrl;
	delete[] platformInfo.modelNumber;
	delete[] platformInfo.dateOfManufacture;
	delete[] platformInfo.platformVersion;
	delete[] platformInfo.operatingSystemVersion;
	delete[] platformInfo.hardwareVersion;
	delete[] platformInfo.firmwareVersion;
	delete[] platformInfo.supportUrl;
	delete[] platformInfo.systemTime;
}

void DuplicateString(char ** targetString, std::string sourceString) {
	*targetString = new char[sourceString.length() + 1];
	strncpy(*targetString, sourceString.c_str(), (sourceString.length() + 1));
}

OCStackResult SetPlatformInfo(std::string platformID,
		std::string manufacturerName, std::string manufacturerUrl,
		std::string modelNumber, std::string dateOfManufacture,
		std::string platformVersion, std::string operatingSystemVersion,
		std::string hardwareVersion, std::string firmwareVersion,
		std::string supportUrl, std::string systemTime) {
	DuplicateString(&platformInfo.platformID, platformID);
	DuplicateString(&platformInfo.manufacturerName, manufacturerName);
	DuplicateString(&platformInfo.manufacturerUrl, manufacturerUrl);
	DuplicateString(&platformInfo.modelNumber, modelNumber);
	DuplicateString(&platformInfo.dateOfManufacture, dateOfManufacture);
	DuplicateString(&platformInfo.platformVersion, platformVersion);
	DuplicateString(&platformInfo.operatingSystemVersion,
			operatingSystemVersion);
	DuplicateString(&platformInfo.hardwareVersion, hardwareVersion);
	DuplicateString(&platformInfo.firmwareVersion, firmwareVersion);
	DuplicateString(&platformInfo.supportUrl, supportUrl);
	DuplicateString(&platformInfo.systemTime, systemTime);

	return OC_STACK_OK;
}

OCStackResult SetDeviceInfo() {
	OCStackResult result = OC_STACK_ERROR;

	OCResourceHandle handle = OCGetResourceHandleAtUri(OC_RSRVD_DEVICE_URI);
	if (handle == NULL) {
		cout << "Failed to find resource " << OC_RSRVD_DEVICE_URI << endl;
		return result;
	}

	result = OCBindResourceTypeToResource(handle, MyBoxConfig::deviceType.c_str());
	if (result != OC_STACK_OK) {
		cout << "Failed to add device type" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_DEVICE_NAME, MyBoxConfig::deviceName);
	if (result != OC_STACK_OK) {
		cout << "Failed to set device name" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_DATA_MODEL_VERSION, MyBoxConfig::dataModelVersions);
	if (result != OC_STACK_OK) {
		cout << "Failed to set data model versions" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_SPEC_VERSION, MyBoxConfig::specVersion);
	if (result != OC_STACK_OK) {
		cout << "Failed to set spec version" << endl;
		return result;
	}

	result = OCPlatform::setPropertyValue(PAYLOAD_TYPE_DEVICE,
			OC_RSRVD_PROTOCOL_INDEPENDENT_ID, MyBoxConfig::protocolIndependentID);
	if (result != OC_STACK_OK) {
		cout << "Failed to set piid" << endl;
		return result;
	}
	return OC_STACK_OK;
}

void PrintUsage() {
	std::cout << "**********************************************" << endl;
	std::cout << "this is mybox server" 							  << endl;
	std::cout << "**********************************************" << endl;
}

static FILE* client_open(const char* path, const char* mode) {
	if (strcmp(path, OC_INTROSPECTION_FILE_NAME) == 0) {
		return fopen("light_introspection.json", mode);
	} else {
		return fopen(SVR_DB_FILE_NAME, mode);
	}
}


int main(int argc, char* argv[]) {
	PrintUsage();
	OCPersistentStorage ps { client_open, fread, fwrite, fclose, unlink };
	// Create PlatformConfig object
	PlatformConfig cfg { OC::ServiceType::InProc, OC::ModeType::Server, &ps };

	cfg.transportType = static_cast<OCTransportAdapter>(OCTransportAdapter::OC_ADAPTER_IP
					| OCTransportAdapter::OC_ADAPTER_TCP);
	cfg.QoS = OC::QualityOfService::LowQos;

	OCPlatform::Configure(cfg);
	OC_VERIFY(OCPlatform::start() == OC_STACK_OK);
	std::cout << "Starting server & setting platform info\n";

	OCStackResult result = SetPlatformInfo(MyBoxConfig::platformId, MyBoxConfig::manufacturerName,
			MyBoxConfig::manufacturerLink, MyBoxConfig::modelNumber, MyBoxConfig::dateOfManufacture, MyBoxConfig::platformVersion,
			MyBoxConfig::operatingSystemVersion, MyBoxConfig::hardwareVersion, MyBoxConfig::firmwareVersion,
			MyBoxConfig::supportLink, MyBoxConfig::systemTime);

	result = OCPlatform::registerPlatformInfo(platformInfo);

	if (result != OC_STACK_OK) {
		std::cout << "Platform Registration failed\n";
		return -1;
	}

	result = SetDeviceInfo();

	if (result != OC_STACK_OK) {
		std::cout << "Device Registration failed\n";
		return -1;
	}

	try {
		// Create the instance of the resource class
		// (in this case instance of class 'LightResource').
		MyboxResource myboxRes;

		// Invoke createResource function of class light.
		myboxRes.createResource();
		std::cout << "Created resource." << std::endl;

		//myLight.addType(std::string("core.brightlight"));
		//myLight.addInterface(std::string(LINK_INTERFACE));
		//std::cout << "Added Interface and Type" << std::endl;

		DeletePlatformInfo();

		// A condition variable will free the mutex it is given, then do a non-
		// intensive block until 'notify' is called on it.  In this case, since we
		// don't ever call cv.notify, this should be a non-processor intensive version
		// of while(true);
		std::mutex blocker;
		std::condition_variable cv;
		std::unique_lock < std::mutex > lock(blocker);
		std::cout << "Waiting" << std::endl;
		cv.wait(lock, [] {return false;});
	} catch (OCException &e) {
		std::cout << "OCException in main : " << e.what() << endl;
	}

	OC_VERIFY(OCPlatform::stop() == OC_STACK_OK);

	return 0;
}
