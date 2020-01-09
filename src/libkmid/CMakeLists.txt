project(libkmid)




########### next target ###############

set(libkmid_LIB_SRCS 
   midiout.cc 
   player.cc 
   track.cc 
   midimapper.cc 
   midfile.cc 
   dattypes.cc 
   midistat.cc 
   deviceman.cc 
   synthout.cc 
   fmout.cc 
   gusout.cc 
   alsaout.cc 
   voiceman.cc 
   mt32togm.cc 
   notearray.cc 
   libkmid.cc )


kde4_add_library(libkmid SHARED ${libkmid_LIB_SRCS})

target_link_libraries(libkmid  ${KDE4_KDECORE_LIBS} )
if (HAVE_LIBASOUND2)
    target_link_libraries(libkmid ${ASOUND_LIBRARY})
endif (HAVE_LIBASOUND2)

set_target_properties(libkmid PROPERTIES VERSION 0.95.0 SOVERSION 0 )
install(TARGETS libkmid  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install( FILES midiout.h player.h track.h midimapper.h  	midfile.h dattypes.h midistat.h deviceman.h synthout.h  	fmout.h gusout.h alsaout.h voiceman.h notearray.h mt32togm.h  	midispec.h libkmid.h  DESTINATION ${INCLUDE_INSTALL_DIR}/libkmid)


