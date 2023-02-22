message(STATUS "+++ FIndOPENCV.cmake")
# set(OPENCV4_DIR_ROOT ${PROJECT_SOURCE_DIR}/third/opencv2)
# set(OPENCV4_INCLUDE_DIRS ${OPENCV4_DIR_ROOT}/include)
# set(OPENCV4_LIB_DIRS ${OPENCV4_DIR_ROOT}/lib)


SET (OPENCV4_INCLUDE_DIRS	${PROJECT_SOURCE_DIR}/third/opencv2/include/opencv4)
SET (OPENCV4_LIBRARIES_DIRS	  ${PROJECT_SOURCE_DIR}/third/opencv2/lib)

#先所有库都依赖上
set(OPENCV4_LINK_LIBS
    opencv_calib3d
    opencv_dnn
    opencv_flann
    opencv_highgui
    opencv_imgproc
    opencv_photo
    opencv_stitching
    opencv_video
    opencv_core
    opencv_features2d
    opencv_gapi
    opencv_imgcodecs
    opencv_ml
    opencv_videoio #以上为opencv4自带modules
    opencv_quality #以下为opencv_contrib增加modules
    opencv_plot
    opencv_fuzzy
    opencv_shape
    opencv_wechat_qrcode
    opencv_hfs
    opencv_phase_unwrapping
    opencv_xphoto
    opencv_surface_matching
    opencv_optflow
    opencv_ximgproc
    opencv_stereo
    opencv_objdetect
    opencv_line_descriptor
    opencv_dnn_superres
    opencv_superres
    opencv_bioinspired
    opencv_barcode
    opencv_freetype
    opencv_mcc
    opencv_dnn_objdetect
    opencv_xfeatures2d
    opencv_structured_light
    opencv_aruco
    opencv_face
    opencv_rgbd
    opencv_rapid
    opencv_text
    opencv_bgsegm
    opencv_ccalib
    opencv_reg
    opencv_datasets
    opencv_xobjdetect
    opencv_dpm
    opencv_tracking
    opencv_intensity_transform
    opencv_img_hash
    opencv_saliency
    opencv_videostab
)
set(OPENCV4_INSTALL_LIBS
    # opencv_calib3d
    # opencv_dnn
    # opencv_flann
    # opencv_highgui
    # opencv_imgproc
    # opencv_photo
    # opencv_stitching
    # opencv_video
    # opencv_core
    # opencv_features2d
    # opencv_gapi
    # opencv_imgcodecs
    # opencv_ml
    # opencv_videoio #以上为opencv4自带modules
    # opencv_quality #以下为opencv_contrib增加modules
    # opencv_plot
    # opencv_fuzzy
    # opencv_shape
    # opencv_wechat_qrcode
    # opencv_hfs
    # opencv_phase_unwrapping
    # opencv_xphoto
    # opencv_surface_matching
    # opencv_optflow
    # opencv_ximgproc
    # opencv_stereo
    # opencv_objdetect
    # opencv_line_descriptor
    # opencv_dnn_superres
    # opencv_superres
    # opencv_bioinspired
    # opencv_barcode
    # opencv_freetype
    # opencv_mcc
    # opencv_dnn_objdetect
    # opencv_xfeatures2d
    # opencv_structured_light
    # opencv_aruco
    # opencv_face
    # opencv_rgbd
    # opencv_rapid
    # opencv_text
    # opencv_bgsegm
    # opencv_ccalib
    # opencv_reg
    # opencv_datasets
    # opencv_xobjdetect
    # opencv_dpm
    # opencv_tracking
    # opencv_intensity_transform
    # opencv_img_hash
    # opencv_saliency
    # opencv_videostab
)


set(OPENCV4_FOUND TRUE)

message(STATUS "--- FIndOPENCV4.cmake")
