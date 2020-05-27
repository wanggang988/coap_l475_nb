/*************author :王刚 *************/
/*************Data :2020/5/27 *************/
/*************version :V1.0 *************/
#include "drv_nb.h"
#include "AT_UART.h"
#include "buffer.h"

static int csq;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart2;
static char sendbuffer[SENDSIZE];
extern struct rx_fifo comlp;
extern UART_HandleTypeDef hlpuart1;
static int attach_network_flag = 0;
extern uint8_t LPUART1InterruptFlag;
//lpuart1 接收缓存区
extern char  recvlp1_buf[LPUART1_BUFFER_SIZE];
extern char * recvlp1_ptr;
extern short  recvlp1_num;

//lpuart1 发送缓存区
extern char   sendlp1_buf[LPUART1_BUFFER_SIZE];
extern char * sendlp1_ptr;
extern short  sendlp1_num;

//uart2 接收缓存区
extern char   recv2_buf[UART2_BUFFER_SIZE];
extern char * recv2_ptr;
extern short  recv2_num;

/* BC28 module echo mode on  */
void bc28_echo_on(void)
{
    Buffer_Lpuart1RecvClear();                                           //清除LPUART1接收缓存区  
    Mcu_UartxSendAtCommand(huart2,"ATE1");                               //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"ATE1");                             //ATE0 echo mode on
    HAL_Delay(500);
}

/* BC28 module echo mode off  */
void bc28_echo_off(void)
{
    Buffer_Lpuart1RecvClear();                                           //清除LPUART1接收缓存区  
    Mcu_UartxSendAtCommand(huart2,"ATE0");                               //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"ATE0");                             //ATE0 echo mode off
    HAL_Delay(500);
}
/* bc28 query signal quality  */
static int csq;
unsigned char bc28_csq_query(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                       //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CSQ");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CSQ");                              
        HAL_Delay(300);
        sscanf((recvlp1_ptr + 2),"+CSQ:%d,",&csq);                       //响应格式为 0x0D0x0A+CSQ:%d,990x0D0x0A,recvlp1_ptr为指针
        
        count++;
        if(count > RETRY_TIMES_LONG)
		{
			flag = 0;
            printf("the csq query is timeout\r\n");
			break;
		}
    }while((csq < 6) || (csq > 32));
    if(count <= RETRY_TIMES_LONG)
    {
        flag = 1;
        printf("get the csq value,the csq is %d\r\n",csq);    
    }
   
    return flag;
}

/* bc28 at test */

unsigned char bc28_at_test(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                    //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT");
        HAL_Delay(300);        
        count++;
        if(count > RETRY_TIMES_LONG)
		{
			flag = 0;
            printf("the at test is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_LONG)
    {
        flag = 1;
        printf("the at test is successed\r\n");    
    }
   
    return flag;
}

/* bc28 reset  */
unsigned char bc28_rst_at(void)
{   
    unsigned char flag;
    Buffer_Lpuart1RecvClear();                                       //清除LPUART1接收缓存区 
    Mcu_UartxSendAtCommand(huart2,"AT+NRB");                         //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"AT+NRB");                              
    HAL_Delay(8000);                                                 //复位后需延时8S
    if(strstr(recvlp1_buf,"OK") != NULL)
    {
        flag = 1;
        printf("bc28 reset is finished\r\n");
    }
    else
    {
        flag = 0;  
    }
    return flag;
}
/* 报告错误代码 */
unsigned char bc28_cmee_at(void)
{   
    unsigned char flag;
    Buffer_Lpuart1RecvClear();                                       //清除LPUART1接收缓存区 
    Mcu_UartxSendAtCommand(huart2,"AT+CMEE=1");                         //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"AT+CMEE=1");                              
    HAL_Delay(300);                                                
    if(strstr(recvlp1_buf,"OK") != NULL)
    {
        flag = 1;
        printf("bc28 report the error code\r\n");
    }
    else
    {
        flag = 0;  
    }
    return flag;
}
/* 设置UE为自动入网模式 */
unsigned char bc28_nconfig_auto(void)
{   
    unsigned char flag;
    Buffer_Lpuart1RecvClear();                                                            //清除LPUART1接收缓存区 
    Mcu_UartxSendAtCommand(huart2,"AT+NCONFIG=AUTOCONNECT,TRUE");                         //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"AT+NCONFIG=AUTOCONNECT,TRUE");                              
    HAL_Delay(300);                                                
    if(strstr(recvlp1_buf,"OK") != NULL)
    {
        flag = 1;
        printf("set bc28 nconfig autoconnect\r\n");
    }
    else
    {
        flag = 0;  
    }
    return flag;
}
/* 设置UE为手动入网模式 */
unsigned char bc28_nconfig_manual(void)
{   
    unsigned char flag;
    Buffer_Lpuart1RecvClear();                                                            //清除LPUART1接收缓存区 
    Mcu_UartxSendAtCommand(huart2,"AT+NCONFIG=AUTOCONNECT,FALSE");                        //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,"AT+NCONFIG=AUTOCONNECT,FALSE");                              
    HAL_Delay(300);                                                
    if(strstr(recvlp1_buf,"OK") != NULL)
    {
        flag = 1;
        printf("set bc28 nconfig manualconnect\r\n");
    }
    else
    {
        flag = 0;  
    }
    return flag;
}

/* 检查USIM卡是否初始化成功 */
unsigned char bc28_cimi_query(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                    //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CIMI");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CIMI");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_LONG)
		{
			flag = 0;
            printf("the cimi query is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"460") == NULL);
    if(count <= RETRY_TIMES_LONG)
    {
        flag = 1;
        printf("the cimi query is successed\r\n");    
    }
   
    return flag;
}
/* 获取NBIOT IMEI信息 */
unsigned char bc28_cgsn_query(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                        //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CGSN=1");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CGSN=1");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the cgsn query is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"+CGSN") == NULL);
    if(count <= RETRY_TIMES_SHORT)
    {
        flag = 1;
        printf("the cgsn query is successed\r\n");    
    }
   
    return flag;
}
/* 查询模块的网络注册状态 */
unsigned char bc28_cereg_query(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                        //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CEREG?");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CEREG?");
        HAL_Delay(1000);
        count++;
        if(count > RETRY_TIMES_LONG)
		{
			flag = 0;
            printf("the bc28 is register failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"+CEREG:0,1") == NULL);
    if(count <= RETRY_TIMES_LONG )
    {
        flag = 1;
        printf("the bc28 register is successed\r\n");    
    }
   
    return flag;    
}
/* 查询模块的网络附着状态 */
unsigned char bc28_agatt_query(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CGATT?");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CGATT?");
        HAL_Delay(1000);
        count++;
        if(count > RETRY_TIMES_LONG)
		{
			flag = 0;
            printf("the bc28 attach network is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"+CGATT:1") == NULL);
    if(count <= RETRY_TIMES_LONG )
    {
        flag = 1;
        printf("the bc28 attach network is successed\r\n");    
    }
   
    return flag;    
}

/* 设置模块的网络附着 */
unsigned char bc28_agatt_set(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CGATT=1");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CGATT=1");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 set attach network is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 set attach network is successed\r\n");    
    }
    return flag;    
}
/* 设置模块的网络去附着 */
unsigned char bc28_deagatt_set(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+CGATT=0");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+CGATT=0");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 set deattach network is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 set deattach network is successed\r\n");    
    }
    return flag;    
}

/* 创建COAP山下文 */
unsigned char bc28_coapcontext_create(int port)
{   
    int count = 0;
    char cmd[256] = {0};
    sprintf(cmd,"AT+QCOAPCREATE=%d",port);
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,cmd);                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,cmd);
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 create coap context is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 create coap context is successed\r\n");    
    }
    return flag;    
}

/* 创建COAP山下文 */
unsigned char bc28_coapcontext_delete(void)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+QCOAPDEL");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+QCOAPDEL");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 delete coap context is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 delete coap context is successed\r\n");    
    }
    return flag;    
}
/* 配置COAP消息头 */
/*******mode :1-5 *******/
/*******msgid :0-65535 *******/
/*******tkl :1-8 *******/
/*******token :十六进制字符串 *******/
unsigned char bc28_coaphead_config(int mode,int msgid,int tkl,char * token)
{   
    int count = 0;
    char cmd[256] = {0};
    if(mode == 1)   //随机生成消息ID和标记值
    {
        sprintf(cmd,"AT+QCOAPHEAD=%d",mode);    
    }
    if(mode == 2)   //随机生成消息ID,配置标记值
    {
        sprintf(cmd,"AT+QCOAPHEAD=%d,%d,%s",mode,tkl,token);    
    }
    if(mode == 3)   //只随机配置消息ID,省略标记值
    {
        sprintf(cmd,"AT+QCOAPHEAD=%d,%d",mode,msgid);    
    }
    if(mode == 4)   //配置消息ID,随机生成标记值
    {
        sprintf(cmd,"AT+QCOAPHEAD=%d,%d",mode,msgid);    
    }
    if(mode == 5)   //配置消息ID和标记值
    {
        sprintf(cmd,"AT+QCOAPHEAD=%d,%d,%d,%s",mode,msgid,tkl,token);    
    }
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,cmd);                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,cmd);
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 set coap header is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 set coap header is successed\r\n");    
    }
    return flag;    
}
/* 配置COAP消息选项 */
/*******opt_count :1-8 *******/
/*******opt_name :0-65535 *******/
/*******opt_value :1-8 *******/
/*******参数个数变化，不便处理，建议直接修改宏定义 *******/
unsigned char bc28_coapmsg_option_config(char * cmd)
{   
    int count = 0;
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,cmd);                                 //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,cmd);
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 set coap message option is failed\r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 set coap message option is successed\r\n");    
    }
    return flag;    
}
/* 发送COAP数据 */
/*******type :0(CON),1:(NON),2:(ACK),3:(RST) *******/
/*******method :1:(GET),2:(POST),3:(PUT),4:(DELETE) *******/
/*******ip_addr :coap服务器地址 *******/
/*******port :coap服务器端口 *******/
/*******参数个数变化，不便处理，建议直接修改宏定义 *******/
void bc28_coapmsg_send(int type,int method,char * ip_addr,int port)
{
    char cmd[256] = {0};
    sprintf(cmd,"AT+QCOAPSEND=%d,%d,%s,%d",type,method,ip_addr,port);
    Buffer_Lpuart1RecvClear();                                           //清除LPUART1接收缓存区  
    Mcu_UartxSendAtCommand(huart2,cmd);                               //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendAtCommand(hlpuart1,cmd);                             
    HAL_Delay(300);
}
/* 发送deviceID和apikeyID */
void bc28_coapmsg_key(char * deviceid,char * apikey)
{
    char cmd[256] = {0};
    sprintf(cmd,"{\"deviceId\":\"%s\",\"apikey\":\"%s\"}",deviceid,apikey);
    Buffer_Lpuart1RecvClear();                                           //清除LPUART1接收缓存区  
    Mcu_UartxSendString(huart2,cmd);                               //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendString(hlpuart1,cmd);                             //ATE0 echo mode on
    HAL_Delay(500);
}

/* Post message */
void bc28_coapmsg_post(char * msg)
{

    Buffer_Lpuart1RecvClear();                                           //清除LPUART1接收缓存区  
    Mcu_UartxSendString(huart2,msg);                               //通过UART2串口输出log信息
    HAL_Delay(10);
    Mcu_UartxSendString(hlpuart1,msg);                             //ATE0 echo mode on
    HAL_Delay(500);
}
/* 发送完数据后发送CTRL+Z */
void nbraw_send(void)
{
	char cmd = 0x1a;
    Buffer_Lpuart1RecvClear();
    Mcu_UartxSendString(huart2,&cmd);                                     //通过UART2串口输出log信息     
    HAL_Delay(10);
    Mcu_UartxSendString(hlpuart1,&cmd);
    HAL_Delay(300);
}

unsigned char bc28_datastatus_query(void)
{   
    int count = 0;
    
    unsigned char flag;
    do{
        Buffer_Lpuart1RecvClear();                                          //清除LPUART1接收缓存区 
        Mcu_UartxSendAtCommand(huart2,"AT+QCOAPDATASTATUS?");                         //通过UART2串口输出log信息
        HAL_Delay(10);
        Mcu_UartxSendAtCommand(hlpuart1,"AT+QCOAPDATASTATUS?");
        HAL_Delay(300);
        count++;
        if(count > RETRY_TIMES_SHORT)
		{
			flag = 0;
            printf("the bc28 do not get the data status \r\n");
			break;
		}
    }while(strstr(recvlp1_buf,"OK") == NULL);
    if(count <= RETRY_TIMES_SHORT )
    {
        flag = 1;
        printf("the bc28 the bc28 get the data status\r\n");    
    }
    return flag;    
}
void nbdatastatus_analyze(void)
{
	int datastatus;
//	at_status status;
	bc28_datastatus_query();
	sscanf((recvlp1_ptr + 2),"+QCOAPDATASTATUS:%d,",&datastatus);
	if(datastatus == 0)
	{
		printf("数据未发送\n");
	}
	else if(datastatus == 1)
	{
		printf("已发送等待IOT回应\n");
	}
	else if(datastatus == 2)
	{
		printf("发送数据失败\n");
	}
	else if(datastatus == 3)
	{
		printf("超时\n");
	}
	else if(datastatus == 4)
	{
		printf(">>>>>>发送成功>>>>>>>\n");
	}
	else if(datastatus == 5)
	{
		printf("获得重置信息\n");
	}
}
char * nbtoken_get(void)
{
    float state;
    int count = 0;
    int tcount=0;
    static char token[16] = {0};
    bc28_coapcontext_create(PORT_ID);
    bc28_coaphead_config(1,13940,4,"01020304");
    bc28_coapmsg_option_config("AT+QCOAPOPTION=3,11,\"/device/auth\",17,50,12,50");
    do{
        bc28_coapmsg_send(0,2,IP_ADD,IP_PORT);
        if(strchr((recvlp1_ptr),'>'))
		{
            bc28_coapmsg_key(RTU_NB_ID,RTU_NB_KEY);
            nbraw_send();  //发送CTRL+Z
            do{
                HAL_Delay(1000 * 2);  
                if(strstr(recvlp1_buf,"token\":\""))
				{
					strncpy(token,strstr((recvlp1_ptr + 2),"token\":\"") + strlen("token\":\""),8);
					printf("Token value is >>>>%s>>>>\n",token);
					if(strlen(token))
						break;
				}
                else if(strstr((recvlp1_ptr + 2),"+QCOAPURC: \"rsp\",2,"))
				{
					sscanf((recvlp1_ptr + 2),"+QCOAPURC: \"rsp\",2,%f,",&state);
					if(state == 2.05f)
					{
						printf(">>>>successful>>>>\n");
					}
					else if(state == 2.03f)
					{
						printf(">>>>无效>>>>\n");
					} 
					else if(state == 4.03f)
					{
						printf(">>>>被禁止>>>>\n");
					} 
					else if(state == 4.01f)
					{
						printf(">>>>未授权>>>>\n");
					} 
					else
						printf(">>>>错误待处理>>>>\n");
					break;
				}
                tcount++;
				if(tcount > RETRY_TIMES_LONG)
					break;
                
            }while(1);
            break;
        }
        else if(count>SENDRDY)
        {
            break;
        } 
        count++;
        HAL_Delay(1000 * 1);
    }while(1);
    return token; 
}

int nbmsg_send(void)
{
    int temp;
	float state;
	uint16_t len,count = 0;
	char *token;
	char tokenvalue[32];
	memset(sendlp1_buf,0,sizeof(sendlp1_buf));
	if(attach_network_flag)
	{
		token = nbtoken_get();
		if(strlen(token))
		{
			convert_tohexstring(tokenvalue,token,strlen(token));
			bc28_coaphead_config(2,13940,8,tokenvalue);
            bc28_coapmsg_option_config("AT+QCOAPOPTION=3,11,\"/device/datapoints\",17,42,12,42");

			do{
                bc28_coapmsg_send(0,2,IP_ADD,IP_PORT);
				if(strchr((recvlp1_ptr),'>'))
				{
                    sendlp1_buf[0] = 0x01;   //数据类型1

					len = sprintf((char*)&sendlp1_buf[3],"{\"266726\":{\"L1_LF_1\":70.00}}");
                    sendlp1_buf[1] = len >> 8;
					sendlp1_buf[2] = len & 0xff;
                    bc28_coapmsg_post(sendlp1_buf);
                    nbraw_send();  //发送CTRL+Z

					printf("send msg %s,length %d\n",&sendlp1_buf[3],len);
					HAL_Delay(1000 * 4);

                    nbdatastatus_analyze();
					break;
				}
				else if(strstr((recvlp1_ptr + 2),"+QCOAPURC: \"rsp\",2,"))
				{
					sscanf((recvlp1_ptr + 2),"+QCOAPURC: \"rsp\",2,%f,",&state);
					if(state == 2.05f)
					{
						printf(">>>>successful>>>>\n");
					}
					else if(state == 2.03f)
					{
						printf(">>>>无效>>>>\n");
					} 
					else if(state == 4.03f)
					{
						printf(">>>>被禁止>>>>\n");
					} 
					else if(state == 4.01f)
					{
						printf(">>>>未授权>>>>\n");
					} 
					else
						printf(">>>>错误待处理>>>>\n");
					break;
				}
				else if(count > SENDRDY)
					break;
				count ++ ;
				HAL_Delay(1000 * 1);
			}while(1);
		}
		else
			printf("Get token value fail\n");

        bc28_coapcontext_delete();
//		attach_network_flag = 0;
	}
	return len;    
}

unsigned char attach_network(void)
{
    unsigned flag = 0;
    bc28_echo_off();
    bc28_cmee_at();
    bc28_nconfig_auto();
    bc28_rst_at();

    bc28_cimi_query();
    bc28_cgsn_query();
    bc28_cereg_query();
    bc28_csq_query();
    flag = bc28_agatt_query();
    if(flag == 1)
    {
        printf("NB模组完成入网流程\r\n");
        attach_network_flag = 1;
    }
    else
    {
        printf("NB模组完成入网失败\r\n");
    }
    return flag;
}
