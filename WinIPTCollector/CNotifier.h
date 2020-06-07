#pragma once
#include <ntifs.h>
#include "CPTConfiger.h"
class CNotifier
{
private:
	struct ConfigInfo {
		//unsigned long outPutBuffLen_ = 0;
		PMDL outPutBuffMdl_ = nullptr; // ����ӳ����û�̬��ַ
		unsigned long long userOutPutAddr_ = 0; // �û����ʵ�OutpuBuff�����ַ
		unsigned int outPutBufferLen_ = 0;
	};
public:
	CNotifier() {
	}
	~CNotifier() {
		__try {
			auto bufferNum = 1;
			for (auto i = 0; i < bufferNum; ++i) {
				auto& curConfig = buffs_[i];
				MmUnmapLockedPages((PVOID)curConfig.userOutPutAddr_, curConfig.outPutBuffMdl_);
				IoFreeMdl(curConfig.outPutBuffMdl_);
			}
			MmUnmapLockedPages((PVOID)userTransDataAddr_, transDataBuffMdl_);
			IoFreeMdl(transDataBuffMdl_);
			ExFreePool(transDataBuff_);
		}
		__except(1) {
			DbgPrint("~CNotifier failed");
		}

	}
	int GetIdx() {
		return idx_;
	}
	void SetParam(int idx, CPTConfiger * configer);
	unsigned long long GetUserTransBuff()
	{
		return userTransDataAddr_;
	}
	void *GetSysTransBuff()
	{
		return transDataBuff_;
	}
	unsigned long long GetUserOutputBuff(int idx)
	{
		return buffs_[idx].userOutPutAddr_;
	}
	unsigned int GetOutBufferLen(int idx)
	{
		return buffs_[idx].outPutBufferLen_;
	}
	void OnNotify(void* param);
private:
	int idx_ = -1;
	KDPC dpc_;
	ConfigInfo buffs_[2]; // 
	CPTConfiger* configer_; // ��Ӧ��������
	void* transDataBuff_ = nullptr; // ���û�����buff��Ϣ�ĵ�ַ�������û����Ķ�ȥ��ȡ����
	PMDL transDataBuffMdl_ = nullptr; // mdl
	unsigned long long userTransDataAddr_ = 0; // �û����ʵ�
};

