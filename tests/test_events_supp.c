 /**
  * Copyright 2019 Comcast Cable Communications Management, LLC
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
  *
 */
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <CUnit/Basic.h>
#include "../src/webcfg_param.h"
#include "../src/webcfg.h"
#include "../src/webcfg_multipart.h"
#include "../src/webcfg_helpers.h"
#include "../src/webcfg_db.h"
#include "../src/webcfg_notify.h"
#include <msgpack.h>
#include <curl/curl.h>
#include <base64.h>
#include "../src/webcfg_generic.h"
#include "../src/webcfg_event.h"
#define FILE_URL "/tmp/webcfg_url"

#define UNUSED(x) (void )(x)

char *url = NULL;
char *interface = NULL;
char device_mac[32] = {'\0'};
int numLoops;

char* get_deviceMAC()
{
	char *device_mac = strdup("b42xxxxxxxxx");	
	return device_mac;
}
void setValues(const param_t paramVal[], const unsigned int paramCount, const int setType, char *transactionId, money_trace_spans *timeSpan, WDMP_STATUS *retStatus, int *ccspStatus)
{
	UNUSED(paramVal);
	UNUSED(paramCount);
	UNUSED(setType);
	UNUSED(transactionId);
	UNUSED(timeSpan);
	UNUSED(ccspStatus);
	*retStatus = WDMP_SUCCESS;
	return;
}

void setAttributes(param_t *attArr, const unsigned int paramCount, money_trace_spans *timeSpan, WDMP_STATUS *retStatus)
{
	UNUSED(attArr);
	UNUSED(paramCount);
	UNUSED(timeSpan);
	*retStatus = WDMP_SUCCESS;
	return;
}

int getForceSync(char** pString, char **transactionId)
{
	UNUSED(pString);
	UNUSED(transactionId);
	return 0;
}
int setForceSync(char* pString, char *transactionId,int *session_status)
{
	UNUSED(pString);
	UNUSED(transactionId);
	UNUSED(session_status);
	return 0;
}

char * getParameterValue(char *paramName)
{
	UNUSED(paramName);
	return NULL;
}

char * getSerialNumber()
{
	char *sNum = strdup("1234");
	return sNum;
}

char * getDeviceBootTime()
{
	char *bTime = strdup("152200345");
	return bTime;
}

char * getProductClass()
{
	char *pClass = strdup("Product");
	return pClass;
}

char * getModelName()
{
	char *mName = strdup("Model");
	return mName;
}

char * getConnClientParamName()
{
	char *pName = strdup("ConnClientParamName");
	return pName;
}
char * getFirmwareVersion()
{
	char *fName = strdup("Firmware.bin");
	return fName;
}

char * getRebootReason()
{
	char *reason = strdup("factory-reset");
	return reason;
}

void sendNotification(char *payload, char *source, char *destination)
{
	WEBCFG_FREE(payload);
	WEBCFG_FREE(source);
	UNUSED(destination);
	return;
}

char *get_global_systemReadyTime()
{
	char *sTime = strdup("158000123");
	return sTime;
}

int Get_Webconfig_URL( char *pString)
{
	char *webConfigURL =NULL;
	loadInitURLFromFile(&webConfigURL);
	pString = webConfigURL;
        printf("The value of pString is %s\n",pString);
	return 0;
}

int Set_Webconfig_URL( char *pString)
{
	printf("Set_Webconfig_URL pString %s\n", pString);
	return 0;
}

int registerWebcfgEvent(WebConfigEventCallback webcfgEventCB)
{
	UNUSED(webcfgEventCB);
	return 0;
}

int unregisterWebcfgEvent()
{
	return 0;
}

//privatessid,14464,410448631,ACK,0 (ACK success)
void test_eventACK()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 410448631;
	tmpData->status = strdup("ACK");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,14464,410448631,ACK,0", NULL);
	sleep(1);
}


/*
homessid,14464,410448631,ACK,60 (Timeout event)
*/
void test_eventTimeout()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 410448631;
	tmpData->status = strdup("ACK");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,14464,410448631,ACK,60", NULL);
	sleep(1);
	if(tmpData)
	{
		WEBCFG_FREE(tmpData->name);
		WEBCFG_FREE(tmpData->status);
		WEBCFG_FREE(tmpData->error_details);
		WEBCFG_FREE(tmpData->cloud_trans_id);
		WEBCFG_FREE(tmpData);
	}
}



//portforwarding,14464,410448631,NACK,0 (NACK event)
void test_eventNACK()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 410448631;
	tmpData->status = strdup("NACK");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	set_global_transID("1234567");
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,14464,410448631,NACK,0,pam,192,failed", NULL);
	sleep(1);
	if(tmpData)
	{
		WEBCFG_FREE(tmpData->name);
		WEBCFG_FREE(tmpData->status);
		WEBCFG_FREE(tmpData->error_details);
		WEBCFG_FREE(tmpData->cloud_trans_id);
		WEBCFG_FREE(tmpData);
	}
}


//portforwarding,14464,0,EXPIRE,0 (EXPIRE event)
void test_eventEXPIRE()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 0;
	tmpData->status = strdup("EXPIRE");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;


	multipartdocs_t *multipartdocs = (multipartdocs_t *)malloc(sizeof(multipartdocs_t));
	multipartdocs->name_space = strdup("telemetry");
	multipartdocs->data = (char *)malloc(64);
	multipartdocs->isSupplementarySync = 1;
	multipartdocs->next = NULL;
	int len=0;
	if(readFromFile("/tmp/input.bin", &multipartdocs->data, &len))
	{
		printf("Data: %s\n",multipartdocs->data);
		multipartdocs->data_size = len;
		printf("Size: %ld\n",multipartdocs->data_size);
	}
	else
	{
		printf("Failed to read file\n");
		multipartdocs->data = NULL;
		multipartdocs->data_size = 0;
	}
	set_global_tmp_node(tmpData);
	set_global_transID("1234567");
	set_global_mp(multipartdocs);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,14464,0,EXPIRE,0", NULL);
	sleep(1);
	if(multipartdocs)
	{
		WEBCFG_FREE(multipartdocs->name_space);
		WEBCFG_FREE(multipartdocs->data);
		multipartdocs->data_size = 0;
		WEBCFG_FREE(multipartdocs);
	}
}



//privatessid,0,3097089542 (crash event)
void test_eventCrash()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 3097089542;
	tmpData->status = strdup("CRASH");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("crash");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,0,3097089542", NULL);
	sleep(1);
}

void test_eventCrashNewVersion()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 3097089542;
	tmpData->status = strdup("CRASH");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("crash");
	
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	
	multipartdocs_t *multipartdocs = (multipartdocs_t *)malloc(sizeof(multipartdocs_t));
	multipartdocs->name_space = strdup("telemetry");
	multipartdocs->data = (char*)malloc(64);
	multipartdocs->isSupplementarySync = 1;
	multipartdocs->next = NULL;
	int len=0;
	if(readFromFile("/tmp/input.bin", &multipartdocs->data, &len))
	{
		printf("Data: %s\n",multipartdocs->data);
		multipartdocs->data_size = len;
		printf("Size: %ld\n",multipartdocs->data_size);
	}
	else
	{
		printf("Failed to read file\n");
		multipartdocs->data = NULL;
		multipartdocs->data_size = 0;
	}
	set_global_tmp_node(tmpData);
	set_global_mp(multipartdocs);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,0,0", NULL);
	sleep(1);
	if(multipartdocs)
	{
		WEBCFG_FREE(multipartdocs->name_space);
		WEBCFG_FREE(multipartdocs->data);
		multipartdocs->data_size = 0;
		WEBCFG_FREE(multipartdocs);
	}

	
}

void test_eventCrashSameVersion()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 3097089542;
	tmpData->status = strdup("CRASH");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("crash");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;

	multipartdocs_t *multipartdocs = (multipartdocs_t *)malloc(sizeof(multipartdocs_t));
	multipartdocs->name_space = strdup("telemetry");
	multipartdocs->data = (char *)malloc(64);
	multipartdocs->isSupplementarySync = 1;
	multipartdocs->next = NULL;
	int len=0;
	if(readFromFile("/tmp/input.bin", &multipartdocs->data, &len))
	{
		printf("Data: %s\n",multipartdocs->data);
		multipartdocs->data_size = len;
		printf("Size: %ld\n",multipartdocs->data_size);
	}
	else
	{
		printf("Failed to read file\n");
		multipartdocs->data = NULL;
		multipartdocs->data_size = 0;
	}
	set_global_tmp_node(tmpData);
	set_global_mp(multipartdocs);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,0,12345", NULL);
	sleep(1);
	if(multipartdocs)
	{
		WEBCFG_FREE(multipartdocs->name_space);
		WEBCFG_FREE(multipartdocs->data);
		multipartdocs->data_size = 0;
		WEBCFG_FREE(multipartdocs);
	}

}

void err_invalidACK()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("telemetry");
	tmpData->version = 4210448631;
	tmpData->status = strdup("ACK");
	tmpData->trans_id = 1464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("a2b3c4d5e6f");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("telemetry,14464,410448631,ACK,0", NULL);
	sleep(1);
	if(tmpData)
	{
		WEBCFG_FREE(tmpData->name);
		WEBCFG_FREE(tmpData->status);
		WEBCFG_FREE(tmpData->error_details);
		WEBCFG_FREE(tmpData->cloud_trans_id);
		WEBCFG_FREE(tmpData);
	}
}



//advsecurity,14464,410448631,ACK;enabled,0 (ACK enabled event)
void test_adveventACKEnabled()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("advsecurity");
	tmpData->version = 410448631;
	tmpData->status = strdup("ACK;enabled");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("advsecurity,14464,410448631,ACK;enabled,0", NULL);
	sleep(1);
}

//advsecurity,14464,410448631,ACK;disabled,0 (ACK disabled event)
void test_adveventACKDisabled()
{
	webconfig_tmp_data_t *tmpData = (webconfig_tmp_data_t *)malloc(sizeof(webconfig_tmp_data_t));
	tmpData->name = strdup("advsecurity");
	tmpData->version = 410448631;
	tmpData->status = strdup("ACK;disabled");
	tmpData->trans_id = 14464;
	tmpData->retry_count = 0;
	tmpData->error_code = 0;
	tmpData->error_details = strdup("success");
	tmpData->isSupplementarySync=1;
	tmpData->retry_timestamp=0;
	tmpData->cloud_trans_id=strdup("abcdef");
	tmpData->next = NULL;
	set_global_tmp_node(tmpData);
	numLoops = 2;
	initWebConfigNotifyTask();
	processWebcfgEvents();
	webcfgCallback("advsecurity,14464,410448631,ACK;disabled,0", NULL);
	sleep(1);
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    	CU_add_test( *suite, "ACK Event\n", test_eventACK);
	CU_add_test( *suite, "Timeout Event\n", test_eventTimeout);
	CU_add_test( *suite, "NACK Event\n", test_eventNACK);
	CU_add_test( *suite, "EXPIRE Event\n", test_eventEXPIRE);
	CU_add_test( *suite, "Crash Event\n", test_eventCrash);
	CU_add_test( *suite, "Crash Event New Version\n", test_eventCrashNewVersion);
	CU_add_test( *suite, "Crash Event same version\n", test_eventCrashSameVersion);
	CU_add_test( *suite, "Invalid ACK Event\n",err_invalidACK);
	CU_add_test( *suite, "adv ACK enabled Event\n", test_adveventACKEnabled);
	CU_add_test( *suite, "adv ACK disabled Event\n", test_adveventACKDisabled);
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( )
{
	unsigned rv = 1;
	CU_pSuite suite = NULL;
	if( CUE_SUCCESS == CU_initialize_registry() ) {
	add_suites( &suite );

	if( NULL != suite ) {
	    CU_basic_set_mode( CU_BRM_VERBOSE );
	    CU_basic_run_tests();
	    printf( "\n" );
	    CU_basic_show_failures( CU_get_failure_list() );
	    printf( "\n\n" );
	    rv = CU_get_number_of_tests_failed();
	}

	CU_cleanup_registry();

	}
	return rv;
}
