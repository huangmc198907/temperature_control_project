#ifndef __EDP_KIT_H__
#define __EDP_KIT_H__

#include "Common.h"

/*---------------------------------------------------------------------------*/
#define MOSQ_MSB(A)         (uint8)((A & 0xFF00) >> 8)
#define MOSQ_LSB(A)         (uint8)(A & 0x00FF)
#define BUFFER_SIZE         (0x100) 
#define PROTOCOL_NAME       "EDP"
#define PROTOCOL_VERSION    1

/*----------------------------������-----------------------------------------*/
#define ERR_UNPACK_CONNRESP_REMAIN              -1000
#define ERR_UNPACK_CONNRESP_FLAG                -1001
#define ERR_UNPACK_CONNRESP_RTN                 -1002
#define ERR_UNPACK_PUSHD_REMAIN                 -1010
#define ERR_UNPACK_PUSHD_DEVID                  -1011
#define ERR_UNPACK_PUSHD_DATA                   -1012
#define ERR_UNPACK_SAVED_REMAIN                 -1020
#define ERR_UNPACK_SAVED_TANSFLAG               -1021
#define ERR_UNPACK_SAVED_DEVID                  -1022
#define ERR_UNPACK_SAVED_DATAFLAG               -1023
#define ERR_UNPACK_SAVED_JSON                   -1024
#define ERR_UNPACK_SAVED_PARSEJSON              -1025
#define ERR_UNPACK_SAVED_BIN_DESC               -1026
#define ERR_UNPACK_SAVED_PARSEDESC              -1027
#define ERR_UNPACK_SAVED_BINLEN                 -1028
#define ERR_UNPACK_SAVED_BINDATA                -1029
#define ERR_UNPACK_PING_REMAIN                  -1030
#define ERR_UNPACK_CMDREQ                       -1031
#define ERR_UNPACK_ENCRYPT_RESP                 -1032
#define ERR_UNPACK_SAVEDATA_ACK                 -1033

/*----------------------------��Ϣ����---------------------------------------*/
/* �������� */
#define CONNREQ             0x10
/* ������Ӧ */
#define CONNRESP            0x20
/* ת��(͸��)���� */
#define PUSHDATA            0x30
/* �洢(ת��)���� */
#define SAVEDATA            0x80
/* �洢ȷ�� */
#define SAVEACK             0x90
/* �������� */
#define CMDREQ              0xA0
/* ������Ӧ */
#define CMDRESP             0xB0
/* �������� */
#define PINGREQ             0xC0
/* ������Ӧ */
#define PINGRESP            0xD0
/* �������� */
#define ENCRYPTREQ          0xE0
/* ������Ӧ */
#define ENCRYPTRESP         0xF0

#ifndef NULL
#define NULL (void*)0
#endif

/* SAVEDATA��Ϣ֧�ֵĸ�ʽ���� */
typedef enum {
    kTypeFullJson = 0x01,
    kTypeBin = 0x02,
    kTypeSimpleJsonWithoutTime = 0x03,
    kTypeSimpleJsonWithTime = 0x04,
    kTypeString = 0x05
}SaveDataType;

/*-------------����buffer, ����buffer, EDP���ṹ����-------------------------*/
typedef struct Buffer
{
    uint8*  _data;          /* buffer���� */
    uint32  _write_pos;     /* bufferд��λ�� */
    uint32  _read_pos;      /* buffer��ȡλ�� */
    uint32  _capacity;      /* buffer���� */
}Buffer, SendBuffer, RecvBuffer, EdpPacket;
/*-----------------------------����Buffer�Ľӿ�------------------------------*/
/* 
 * ������:  NewBuffer
 * ����:    ����Buffer
 * ˵��:    һ�������, NewBuffer��DeleteBufferӦ�óɶԳ���
 * ����:    ��
 * ����ֵ:  ���� (Buffer*)
 *          ����ֵ�ǿ� ����Buffer�ɹ�, �������Buffer��ָ��
 *          ����ֵΪ�� ����Bufferʧ��, �ڴ治��
 */
Buffer* NewBuffer(void);
/* 
 * ������:  DeleteBuffer
 * ����:    ����Buffer
 * ˵��:    һ�������, NewBuffer��DeleteBufferӦ�óɶԳ���
 * ����:    buf     һ��Buffer��ָ���ָ��
 * ����ֵ:  ��
 */
void DeleteBuffer(Buffer** buf);
/* 
 * ������:  CheckCapacity
 * ����:    ���Buffer�Ƿ��ܹ�д�볤��Ϊlen���ֽ���, 
 *          ���Buffer����������, �Զ��ɱ���չBuffer������(��Ӱ��Buffer����)
 * ����:    buf     ��Ҫд��Buffer��ָ��
 *          len     ����д��ĳ���
 * ����ֵ:  ���� (int32)
 *          <0      ʧ��, �ڴ治��
 *          =0      �ɹ�
 */
int32 CheckCapacity(Buffer* buf, uint32 len);


/*-----------------------------�ͻ��˲����Ľӿ�------------------------------*/
/* 
 * ������:  EdpPacketType 
 * ����:    ��ȡһ��EDP������Ϣ����, �ͻ����������Ϣ��������ͬ�Ĵ���
 * ��غ���:Unpack***�ຯ��
 * ����:    pkg         EDPЭ���
 * ����ֵ:  ���� (uint8)
 *          ֵ          ��Ϣ����(��ϸ�μ���h����Ϣ���Ͷ���)
 */
/* ����:
 * ...
 * int8 mtype = EdpPacketType(pkg);
 * switch(mtype)
 * {
 *  case CONNRESP:
 *      UnpackConnectResp(pkg);
 *      break;
 *  case PUSHDATA:
 *      UnpackPushdata(pkg, src_devid, data, data_len);
 *      break;
 *  case SAVEDATA:
 *      UnpackSavedata(pkg, src_devid, flag, data);
 *      break;
 *  case PINGRESP:
 *      UnpackPingResp(pkg); 
 *      break;
 *  ...
 * }
 */
uint8 EdpPacketType(EdpPacket* pkg);
/* 
 * ������:  PacketConnect1 
 * ����:    ��� ���豸���豸�Ƶ�EDPЭ���, �����豸�Ƶ�����(��¼��֤��ʽ1)
 * ˵��:    ���ص�EDP�����͸��豸�ƺ�, ��Ҫ�ͻ�����ɾ���ð�
 *          �豸�ƻ�ظ�������Ӧ���豸
 * ��غ���:UnpackConnectResp
 * ����:    devid       �豸ID, �����豸ʱƽ̨���ص�ID
 *          auth_key    ��Ȩ��Ϣ(api-key), ��ƽ̨����Ŀ��Բ������豸��api-key�ַ���
 * ����ֵ:  ���� (EdpPacket*) 
 *          �ǿ�        EDPЭ���
 *          Ϊ��        EDPЭ�������ʧ�� 
 */
EdpPacket* PacketConnect1(const char* devid, const char* auth_key);

/* 
 * ������:  PacketConnect2 
 * ����:    ��� ���豸���豸�Ƶ�EDPЭ���, �����豸�Ƶ�����(��¼��֤��ʽ2)
 * ˵��:    ���ص�EDP�����͸��豸�ƺ�, ��Ҫ�ͻ�����ɾ���ð�
 *          �豸�ƻ�ظ�������Ӧ���豸
 * ��غ���:UnpackConnectResp
 * ����:    userid      �û�ID, ��ƽ̨ע���˺�ʱƽ̨���ص��û�ID
 *          auth_info   ��Ȩ��Ϣ, ��ƽ̨�����豸ʱ��д�豸��auth_info����
 *                      (json�����ַ���), ��������Ҫ�߱�Ψһ��
 * ����ֵ:  ���� (EdpPacket*) 
 *          �ǿ�        EDPЭ���
 *          Ϊ��        EDPЭ�������ʧ�� 
 */
EdpPacket* PacketConnect2(const char* userid, const char* auth_info);

/* 
 * ������:  PacketPushdata
 * ����:    ��� �豸���豸�Ƶ�EDPЭ���, �豸���豸֮��ת������
 * ˵��:    ���ص�EDP�����͸��豸�ƺ�, ��Ҫɾ�������
 * ��غ���:UnpackPushdata
 * ����:    dst_devid   Ŀ���豸ID
 *          data        ����
 *          data_len    ���ݳ���
 * ����ֵ:  ���� (EdpPacket*) 
 *          �ǿ�        EDPЭ���
 *          Ϊ��        EDPЭ�������ʧ�� 
 */
EdpPacket* PacketPushdata(const char* dst_devid, 
        const char* data, uint32 data_len);

/* 
 * ������:  UnpackPushdata
 * ����:    ��� ���豸�Ƶ��豸��EDPЭ���, �豸���豸֮��ת������
 * ˵��:    �����豸�Ʒ���������, ͨ������GetEdpPacket��EdpPacketType�жϳ���pushdata��, 
 *          ��������ӦEDP����Ϊ����, �ɸú������н��� 
 *          ���ص�Դ�豸ID(src_devid)������(data)����Ҫ�ͻ����ͷ�
 * ��غ���:PacketPushdata, GetEdpPacket, EdpPacketType
 * ����:    pkg         EDP��, ������pushdata��
 *          src_devid   Դ�豸ID
 *          data        ����
 *          data_len    ���ݳ���
 * ����ֵ:  ���� (int32) 
 *          =0          �����ɹ�
 *          <0          ����ʧ��, ����ʧ��ԭ�����h�ļ��Ĵ�����
 */
int32 UnpackPushdata(EdpPacket* pkg, char** src_devid, 
        char** data, uint32* data_len);


/* 
 * ������:  PacketSavedataSimpleString
 * ����:    ��� �豸���豸�Ƶ�EDPЭ���, �洢����(�Էֺŷָ��ļ��ַ�����ʽ)
 * ˵��:    ���ص�EDP�����͸��豸�ƺ�, ��Ҫɾ�������
 * ��غ���:UnpackSavedataSimpleString
 * ����:    dst_devid   Ŀ���豸ID
 *          input       �Էֺŷָ��ļ��ַ�����ʽ��
 *                      ������豸�ն˽���Э��2-EDP.docx��
 * ����ֵ:  ���� (EdpPacket*) 
 *          �ǿ�        EDPЭ���
 *          Ϊ��        EDPЭ�������ʧ�� 
 */
EdpPacket* PacketSavedataSimpleString(const char* dst_devid, const char* input);

/* 
 * ������:  PacketCmdResp
 * ����:    ����������������Ӧ
 * ˵��:    ���ص�EDP�����͸��豸�ƺ�, ��Ҫ�ͻ�����ɾ���ð�
 *          
 * ��غ���:UnpackCmdReq
 * ����:    cmdid       ����id
 *          cmdid_len   ����id����
 *          resp        ��Ӧ����Ϣ
 *          resp_len    ��Ӧ��Ϣ����
 * ����ֵ:  ���� (EdpPacket*) 
 *          �ǿ�        EDPЭ���
 *          Ϊ��        EDPЭ�������ʧ�� 
 */
EdpPacket* PacketCmdResp(const char* cmdid, uint16 cmdid_len, 
        const char* resp, uint32 resp_len);

/* 
 * ������:  UnpackCmdReq
 * ����:    ��� ���豸�Ƶ��豸��EDPЭ���, ����������Ϣ
 * ˵��:    �����豸�Ʒ���������, ��������������Ϣ��
 *          ��ȡ��cmdid�Լ�req��Ҫ��ʹ�ú��ͷš�
 * ��غ���:PacketCmdResp
 * ����:    pkg         EDP��
 *          cmdid       ��ȡ����id
 *          cmdid_len   cmdid�ĳ���
 *          req         �û��������ʼλ��
 *          req_len     �û�����ĳ���
 * ����ֵ:  ���� (int32) 
 *          =0          �����ɹ�
 *          <0          ����ʧ��, ����ʧ��ԭ�����h�ļ��Ĵ�����
 */
int32 UnpackCmdReq(EdpPacket* pkg, char** cmdid, uint16* cmdid_len, 
			      char** req, uint32* req_len);


EdpPacket* PacketSaveJson(const char* dst_devid, char* json_obj, SaveDataType type);

EdpPacket* PacketSaveBin(const char* dst_devid, char* desc_obj, unsigned int bin_len);


#endif /* __EDP_KIT_H__ */
