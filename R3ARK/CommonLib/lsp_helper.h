#pragma once
#include "CommonCPP/Common.h"
#include <Ws2spi.h>  
#include <Sporder.h>
#include <guiddef.h>
namespace usr::util
{
	class lsp_helper:public Singleton<lsp_helper>
	{
	public:
		lsp_helper() {};
		~lsp_helper() {};
	private:
		LPWSAPROTOCOL_INFOW GetProvider(LPINT lpnTotalProtocols)
		{
			DWORD dwSize = 0;
			int nError;
			LPWSAPROTOCOL_INFOW pProtoInfo = NULL;

			// ȡ����Ҫ�ĳ���  
			if (::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError) == SOCKET_ERROR)
			{
				if (nError != WSAENOBUFS)
					return NULL;
			}

			pProtoInfo = (LPWSAPROTOCOL_INFOW)::GlobalAlloc(GPTR, dwSize);
			*lpnTotalProtocols = ::WSCEnumProtocols(NULL, pProtoInfo, &dwSize, &nError);
			return pProtoInfo;
		}
	public:
		bool install_lsp(_tstring lsp_name, _tstring file_path,GUID lsp_guid)
		{
			LPWSAPROTOCOL_INFOW pProtoInfo=nullptr;
			int nProtocols;
			WSAPROTOCOL_INFOW OriginalProtocolInfo[3] = {};
			DWORD             dwOrigCatalogId[3] = {};
			int nArrayCount = 0;
			DWORD dwLayeredCatalogId;        // ���Ƿֲ�Э���Ŀ¼ID��  
			int nError;

			// �ҵ����ǵ��²�Э�飬����Ϣ����������  
			// ö�����з�������ṩ��  
			pProtoInfo = GetProvider(&nProtocols);
			auto free_info = [&]() {
				if (pProtoInfo)
				{
					::GlobalFree(pProtoInfo);
				}
			};
			auto exit = std::experimental::make_scope_exit([&]() {
				if (pProtoInfo)
				{
					::GlobalFree(pProtoInfo);
				}
			});
			bool bFindUdp = false;
			bool bFindTcp = false;
			bool bFindRaw = false;
			for (int i = 0; i < nProtocols; i++)
			{
				if (pProtoInfo[i].iAddressFamily == AF_INET)
				{
					if (!bFindUdp && pProtoInfo[i].iProtocol == IPPROTO_UDP)
					{
						memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
						OriginalProtocolInfo[nArrayCount].dwServiceFlags1 =
							OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES);

						dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
						bFindUdp = true;
					}
					if (!bFindTcp && pProtoInfo[i].iProtocol == IPPROTO_TCP)
					{
						memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
						OriginalProtocolInfo[nArrayCount].dwServiceFlags1 =
							OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES);

						dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
						bFindTcp = true;
					}
					if (!bFindRaw && pProtoInfo[i].iProtocol == IPPROTO_IP)
					{
						memcpy(&OriginalProtocolInfo[nArrayCount], &pProtoInfo[i], sizeof(WSAPROTOCOL_INFOW));
						OriginalProtocolInfo[nArrayCount].dwServiceFlags1 =
							OriginalProtocolInfo[nArrayCount].dwServiceFlags1 & (~XP1_IFS_HANDLES);

						dwOrigCatalogId[nArrayCount++] = pProtoInfo[i].dwCatalogEntryId;
						bFindRaw = true;
					}
				}
			}
			// ��װ���ǵķֲ�Э�飬��ȡһ��dwLayeredCatalogId  
			// �����һ���²�Э��Ľṹ���ƹ�������  
			WSAPROTOCOL_INFOW LayeredProtocolInfo;
			memcpy(&LayeredProtocolInfo, &OriginalProtocolInfo[0], sizeof(WSAPROTOCOL_INFOW));
			// �޸�Э�����ƣ����ͣ�����PFL_HIDDEN��־  
			_tcscpy_s(LayeredProtocolInfo.szProtocol, lsp_name.c_str());
			LayeredProtocolInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL; // 0;  
			LayeredProtocolInfo.dwProviderFlags |= PFL_HIDDEN;
			// ��װ  
			if (::WSCInstallProvider(&lsp_guid,
				file_path.c_str(), &LayeredProtocolInfo, 1, &nError) == SOCKET_ERROR)
			{
				_tprintf(_T("path=%s, error=%d\n"), file_path.c_str(), nError);
				return false;
			}
			// ����ö��Э�飬��ȡ�ֲ�Э���Ŀ¼ID��  
			free_info();
			pProtoInfo = GetProvider(&nProtocols);
			for (int i = 0; i < nProtocols; i++)
			{
				if (memcmp(&pProtoInfo[i].ProviderId, &lsp_guid, sizeof(lsp_guid)) == 0)
				{
					dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
					break;
				}
			}
			// ��װЭ����  
			// �޸�Э�����ƣ�����  
			WCHAR wszChainName[WSAPROTOCOL_LEN + 1];
			for (int i = 0; i < nArrayCount; i++)
			{
				_tcprintf_s(wszChainName,_T("%s over %s"), lsp_name.c_str(), OriginalProtocolInfo[i].szProtocol);
				_tcscpy_s(OriginalProtocolInfo[i].szProtocol, wszChainName);
				if (OriginalProtocolInfo[i].ProtocolChain.ChainLen == 1)
				{
					OriginalProtocolInfo[i].ProtocolChain.ChainEntries[1] = dwOrigCatalogId[i];
				}
				else
				{
					for (int j = OriginalProtocolInfo[i].ProtocolChain.ChainLen; j > 0; j--)
					{
						OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j]
							= OriginalProtocolInfo[i].ProtocolChain.ChainEntries[j - 1];
					}
				}
				OriginalProtocolInfo[i].ProtocolChain.ChainLen++;
				OriginalProtocolInfo[i].ProtocolChain.ChainEntries[0] = dwLayeredCatalogId;
			}
			// ��ȡһ��Guid����װ֮  
			GUID ProviderChainGuid;
			if (::UuidCreate(&ProviderChainGuid) == RPC_S_OK)
			{
				if (::WSCInstallProvider(&ProviderChainGuid,
					file_path.c_str(), OriginalProtocolInfo, nArrayCount, &nError) == SOCKET_ERROR)
				{
					return false;
				}
			}
			else
				return false;
			// ��������WinsockĿ¼�������ǵ�Э������ǰ  
			// ����ö�ٰ�װ��Э��  
			free_info();
			pProtoInfo = GetProvider(&nProtocols);
			PDWORD dwIds = (PDWORD)malloc(sizeof(DWORD) * nProtocols);
			int nIndex = 0;
			// ������ǵ�Э����  
			for (int i = 0; i < nProtocols; i++)
			{
				if ((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
					(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
					dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
			}
			// �������Э��  
			for (int i = 0; i < nProtocols; i++)
			{
				if ((pProtoInfo[i].ProtocolChain.ChainLen <= 1) ||
					(pProtoInfo[i].ProtocolChain.ChainEntries[0] != dwLayeredCatalogId))
					dwIds[nIndex++] = pProtoInfo[i].dwCatalogEntryId;
			}
			// ��������WinsockĿ¼  
			if ((nError = ::WSCWriteProviderOrder(dwIds, nIndex)) != ERROR_SUCCESS)
			{
				return false;
			}
			return true;
		}
		bool remov_lsp(GUID lsp_guid)
		{
			LPWSAPROTOCOL_INFOW pProtoInfo;
			int nProtocols;
			DWORD dwLayeredCatalogId;
			// ����Guidȡ�÷ֲ�Э���Ŀ¼ID��  
			pProtoInfo = GetProvider(&nProtocols);
			auto exit = std::experimental::make_scope_exit([&]() {
				if (pProtoInfo)
				{
					::GlobalFree(pProtoInfo);
				}
			});
			int nError;
			int i;
			for (i = 0; i < nProtocols; i++)
			{
				if (memcmp(&lsp_guid, &pProtoInfo[i].ProviderId, sizeof(lsp_guid)) == 0)
				{
					dwLayeredCatalogId = pProtoInfo[i].dwCatalogEntryId;
					break;
				}
			}
			if (i < nProtocols)
			{
				// �Ƴ�Э����  
				for (i = 0; i < nProtocols; i++)
				{
					if ((pProtoInfo[i].ProtocolChain.ChainLen > 1) &&
						(pProtoInfo[i].ProtocolChain.ChainEntries[0] == dwLayeredCatalogId))
					{
						::WSCDeinstallProvider(&pProtoInfo[i].ProviderId, &nError);
					}
				}

				// �Ƴ��ֲ�Э��  
				::WSCDeinstallProvider(&lsp_guid, &nError);
			}
			else return false;
			return true;
		}
	};
}