/*!
 * \copy
 *     Copyright (c)  2009-2013, Cisco Systems
 *     All rights reserved.
 *
 *
 *
 *************************************************************************/
#if !defined(CUSTOM_ENCODER_H)
#define CUSTOM_ENCODER_H


#include "codec_api.h"
#include "codec_def.h"
#include "codec_app_def.h"

//#define OUTPUT_BIT_STREAM
//#define DUMP_SRC_PICTURE
//#define REC_FRAME_COUNT

#define ENABLE_DEBUG_CODE 1
#define ENABLE_TRACE 0

class ISVCEncoder;
namespace WelsEnc {
class CustomH264Encoder : public ISVCEncoder {
 public:
  CustomH264Encoder();
  virtual ~CustomH264Encoder();

  /* Interfaces override from ISVCEncoder */
  /*
   * return: CM_RETURN: 0 - success; otherwise - failed;
   */
  virtual int EXTAPI Initialize (const SEncParamBase* argv);
  virtual int EXTAPI InitializeExt (const SEncParamExt* argv);

  virtual int EXTAPI GetDefaultParams (SEncParamExt* argv);

  virtual int EXTAPI Uninitialize();

  /*
   * return: 0 - success; otherwise - failed;
   */
  virtual int EXTAPI EncodeFrame (const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo);
  virtual int        EncodeFrameInternal (const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo);

  /*
   * return: 0 - success; otherwise - failed;
   */
  virtual int EXTAPI EncodeParameterSets (SFrameBSInfo* pBsInfo);
  /*
   * return: 0 - success; otherwise - failed;
   */
  virtual int EXTAPI ForceIntraFrame (bool bIDR, int iLayerId = -1);

  /************************************************************************
   * InDataFormat, IDRInterval, SVC Encode Param, Frame Rate, Bitrate,..
   ************************************************************************/
  /*
   * return: CM_RETURN: 0 - success; otherwise - failed;
   */
  virtual int EXTAPI SetOption (ENCODER_OPTION opt_id, void* option);
  virtual int EXTAPI GetOption (ENCODER_OPTION opt_id, void* option);

 private:

  int InitializeInternal ();
  int           m_iMaxPicWidth;
  int           m_iMaxPicHeight;

  int           m_iCspInternal;
  bool              m_bInitialFlag;


#if ENABLE_TRACE  
  void TraceParamInfo(SEncParamExt *pParam);
  void LogStatistics (const int64_t kiCurrentFrameTs,int32_t iMaxDid);
  void UpdateStatistics(SFrameBSInfo* pBsInfo, const int64_t kiCurrentFrameMs);

  sWelsEncCtx*      m_pEncContext;

  welsCodecTrace*   m_pWelsTrace;
#endif // ENABLE_TRACE

#ifdef OUTPUT_BIT_STREAM
  FILE*             m_pFileBs;
  FILE*             m_pFileBsSize;
  bool              m_bSwitch;
  int32_t           m_iSwitchTimes;
#endif//OUTPUT_BIT_STREAM

#ifdef REC_FRAME_COUNT
  int32_t           m_uiCountFrameNum;
#endif//REC_FRAME_COUNT

  void    InitEncoder (void);
  void    DumpSrcPicture (const SSourcePicture*  pSrcPic, const int iUsageType);
};
}
#endif // !defined(CUSTOM_ENCODER_H)
