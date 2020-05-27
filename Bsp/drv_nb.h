#ifndef __DRV_NB_H__
#define __DRV_NB_H__
#include "main.h"

#define PORT_ID  (56830)
#define RTU_NB_ID   "266726"     // ƽ̨���ɵ�ID
#define RTU_NB_KEY   "96379ee9f7d97c2fd7ec"   //key�̶�����20ƽ̨���ɵ�
#define IP_ADD "219.142.70.215" //coap�������ϱ���ַ��
#define IP_PORT (5685) //coap�������ϱ���ַ��

#define SENDRDY (3)         //�ȴ�>
#define POLLAT (30)         //�ȴ���������,ע��,�ź�����poll
#define SENDSIZE (512)
#define RETRY_TIMES_LONG     (30)      //�����Ҫ��β�ѯ�����
#define RETRY_TIMES_SHORT    (5)       //��Բ���Ҫ��β�ѯ�����
void convert_tohexstring(char* dst,char* src,uint8_t len);
void nbdevice_reset(void);
void bc28_echo_on(void);
void bc28_echo_off(void);
unsigned char bc28_csq_query(void);
unsigned char bc28_rst_at(void);
unsigned char bc28_cmee_at(void);
unsigned char bc28_nconfig_auto(void);
unsigned char bc28_nconfig_manual(void);
unsigned char bc28_at_test(void);
unsigned char bc28_cimi_query(void);
unsigned char bc28_cgsn_query(void);
unsigned char bc28_cereg_query(void);
unsigned char bc28_agatt_query(void);
unsigned char bc28_agatt_set(void);
unsigned char bc28_deagatt_set(void);
unsigned char bc28_coapcontext_create(int port);
unsigned char bc28_coapcontext_delete(void);
unsigned char bc28_coaphead_config(int mode,int msgid,int tkl,char * token);
unsigned char bc28_coapmsg_option_config(char * cmd);
unsigned char bc28_datastatus_query(void);
void nbraw_send(void);
char * nbtoken_get(void);
void nbdatastatus_analyze(void);
unsigned char attach_network(void);
void bc28_coapmsg_post(char * msg);
int nbmsg_send(void);
#endif





