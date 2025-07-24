/*!
 * \copy
 *     Copyright (c)  2013, Cisco Systems
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in
 *          the documentation and/or other materials provided with the
 *          distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *     COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <assert.h>
#include "customEncoder.h"
#include "typedefs.h"
#include "utils.h"
#include "macros.h"
#include "version.h"
#include "codec_ver.h"
//#include "ref_list_mgr_svc.h"

#include <time.h>
#include <measure_time.h>
#include <sys/time.h>

#if ENABLE_DEBUG_CODE
#include <cstdio>
#endif

namespace WelsEnc {

/*
 *  CustomH264Encoder class implementation
 */
CustomH264Encoder::CustomH264Encoder() {
  InitEncoder();
}

CustomH264Encoder::~CustomH264Encoder() {
  Uninitialize();
}

void CustomH264Encoder::InitEncoder (void) {
  /// not yet implement 
}

/* Interfaces override from ISVCEncoder */

int CustomH264Encoder::GetDefaultParams (SEncParamExt* argv) {
  /// not yet implement 
  return cmResultSuccess;
}

/*
 *  SVC Encoder Initialization
 */
int CustomH264Encoder::Initialize (const SEncParamBase* argv) {

  return InitializeInternal();
}

int CustomH264Encoder::InitializeExt (const SEncParamExt* argv) {

  return InitializeInternal ();
}

int CustomH264Encoder::InitializeInternal () {
  
// need to implement here  
#if 0
  if (WelsInitEncoderExt (&m_pEncContext, pCfg, &m_pWelsTrace->m_sLogCtx, NULL)) {
    WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_ERROR, "CustomH264Encoder::Initialize(), WelsInitEncoderExt failed.");
    WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_DEBUG,
             "Problematic Input Base Param: iUsageType=%d, Resolution=%dx%d, FR=%f, TLayerNum=%d, DLayerNum=%d",
             pCfg->iUsageType, pCfg->iPicWidth, pCfg->iPicHeight, pCfg->fMaxFrameRate, pCfg->iTemporalLayerNum,
             pCfg->iSpatialLayerNum);
    Uninitialize();
    return cmInitParaError;
  }
#endif
  m_bInitialFlag  = true;

  return cmResultSuccess;
}

/*
 *  SVC Encoder Uninitialization
 */
int /*int32_t*/ CustomH264Encoder::Uninitialize() {
  if (!m_bInitialFlag) {
    return 0;
  }
  /// not yet implement 
  m_bInitialFlag = false;

  return 0;
}


/*
 *  SVC core encoding
 */
int CustomH264Encoder::EncodeFrame (const SSourcePicture* kpSrcPic, SFrameBSInfo* pBsInfo) {

  const int32_t kiEncoderReturn = EncodeFrameInternal (kpSrcPic, pBsInfo);

  if (kiEncoderReturn != cmResultSuccess) {
    return kiEncoderReturn;
  }

#ifdef REC_FRAME_COUNT
  ++ m_uiCountFrameNum;
  WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_INFO,
           "CustomH264Encoder::EncodeFrame(), m_uiCountFrameNum= %d,", m_uiCountFrameNum);
#endif//REC_FRAME_COUNT

  return kiEncoderReturn;
}


int CustomH264Encoder ::EncodeFrameInternal (const SSourcePicture*  pSrcPic, SFrameBSInfo* pBsInfo) {
 
  // not yet implement, need to review again here
#if ENABLE_DEBUG_CODE
  printf("      ---> beginning encoded frame \r\n");
#endif  

#if 0
  if ((pSrcPic->iPicWidth < 16) || ((pSrcPic->iPicHeight < 16))) {
    //WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_ERROR, "Don't support width(%d) or height(%d) which is less than 16!",
    //         pSrcPic->iPicWidth, pSrcPic->iPicHeight);
    return cmUnsupportedData;
  }

  const int64_t kiBeforeFrameUs = WelsTime();
  const int32_t kiEncoderReturn = WelsEncoderEncodeExt (m_pEncContext, pBsInfo, pSrcPic);
  const int64_t kiCurrentFrameMs = (WelsTime() - kiBeforeFrameUs) / 1000;
  if ((kiEncoderReturn == ENC_RETURN_MEMALLOCERR) || (kiEncoderReturn == ENC_RETURN_MEMOVERFLOWFOUND)
      || (kiEncoderReturn == ENC_RETURN_VLCOVERFLOWFOUND)) {
    WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_DEBUG, "CustomH264Encoder::EncodeFrame() not succeed, err=%d",
             kiEncoderReturn);
    WelsUninitEncoderExt (&m_pEncContext);
    return cmMallocMemeError;
  } else if ((kiEncoderReturn != ENC_RETURN_SUCCESS) && (kiEncoderReturn == ENC_RETURN_CORRECTED)) {
    WelsLog (&m_pWelsTrace->m_sLogCtx, WELS_LOG_ERROR, "unexpected return(%d) from EncodeFrameInternal()!",
             kiEncoderReturn);
    return cmUnknownReason;
  }

  UpdateStatistics (pBsInfo, kiCurrentFrameMs);

 #endif

#if ENABLE_DEBUG_CODE
  printf("      ---> finished encoded frame \r\n");
#endif  

  return cmResultSuccess;

}

int CustomH264Encoder::EncodeParameterSets (SFrameBSInfo* pBsInfo) {
  /// not yet implement 
  return 0;
}

/*
 *  Force key frame
 */
int CustomH264Encoder::ForceIntraFrame (bool bIDR, int iLayerId) {
  /// not yet implement 
  return 0;
}
#if ENABLE_TRACE  
void CustomH264Encoder::TraceParamInfo (SEncParamExt* pParam) {
  /// not yet implement 
}

void CustomH264Encoder::LogStatistics (const int kiCurrentFrameTs, int iMaxDid) {
  /// not yet implement 
}

void CustomH264Encoder::UpdateStatistics (SFrameBSInfo* pBsInfo,
    const int kiCurrentFrameMs) {
  /// not yet implement 
}
#endif
/************************************************************************
* InDataFormat, IDRInterval, SVC Encode Param, Frame Rate, Bitrate,..
************************************************************************/
int CustomH264Encoder::SetOption (ENCODER_OPTION eOptionId, void* pOption) { 
  /// not yet implement 
  return 0;
}

int CustomH264Encoder::GetOption (ENCODER_OPTION eOptionId, void* pOption) {
  /// not yet implement 
  return 0;
}

void CustomH264Encoder::DumpSrcPicture (const SSourcePicture*  pSrcPic, const int iUsageType) {
  /// not yet implement 
  return;
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
