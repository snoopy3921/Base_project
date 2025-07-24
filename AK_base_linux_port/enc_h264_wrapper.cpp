#include "enc_h264_wrapper.h"

void EncoderTaskInternal() {
    //## Encoder handler
    ISVCEncoder* encoder = nullptr;
    int iCacheLineSize     = 16;    
    SEncParamBase param;
    param.iPicHeight       = HEIGHT;
    param.iPicWidth        = WIDTH;
    param.iTargetBitrate   = BITRATE;
    param.fMaxFrameRate    = FRAMERATE;
    param.iRCMode          = RC_BITRATE_MODE;
    param.iUsageType       = CAMERA_VIDEO_REAL_TIME;

    CMemoryAlign* pMemA    = new CMemoryAlign (iCacheLineSize);

    //encoder = new CWelsH264SVCEncoder(); // default h264 encoder API
    encoder = new CustomH264Encoder();     // custome simple h264 encoder API
  
    if(encoder == nullptr) {
        printf("[EncoderTaskInternal] Error !! encoder handler is null\r\n");
        return ;
    }

    encoder->Initialize(&param);

    // output h264
    SFrameBSInfo output;        
    memset (&output, 0, sizeof (SFrameBSInfo));
    
    
    //# config input yuv     
    SSourcePicture* pSrcPic = new SSourcePicture;    
    pSrcPic->iColorFormat   = videoFormatI420;
    pSrcPic->uiTimeStamp    = 0;
    pSrcPic->iPicWidth      = WIDTH;
    pSrcPic->iPicHeight     = HEIGHT;

    uint32_t iSourceWidth = pSrcPic->iPicWidth;
    uint32_t iSourceHeight = pSrcPic->iPicHeight;
    uint32_t kiPicResSize = iSourceWidth * iSourceHeight * 3 >> 1;
    uint8_t* pYUV = new uint8_t [kiPicResSize];

    // fill raw data to *pYUV
    // **************** 
    // ReadYUVFrame
    // ==================
    //update pSrcPic
    pSrcPic->iStride[0] = iSourceWidth;
    pSrcPic->iStride[1] = pSrcPic->iStride[2] = pSrcPic->iStride[0] >> 1;

    pSrcPic->pData[0] = pYUV;
    pSrcPic->pData[1] = pSrcPic->pData[0] + (iSourceWidth * iSourceHeight);
    pSrcPic->pData[2] = pSrcPic->pData[1] + (iSourceWidth * iSourceHeight >> 2);


    // Encode the input raw data to output h264
    encoder->EncodeFrame (pSrcPic, &output);

/*
        for (int i = 0; i < info.iLayerNum; ++i) {
            SLayerBSInfo& layer = info.sLayerInfo[i];
            for (int j = 0; j < layer.iNalCount; ++j) {
                uint8_t* pNal = layer.pBsBuf;
                int      len  = layer.pNalLengthInByte[j];
                SendEncodedData(pNal, len);
            }
        }
*/

    encoder->Uninitialize();

    // Destroy encoder
    if (encoder) {
      delete encoder;
      encoder = nullptr;
    }
}
