##### Primary Build Targets #####
	
### Programs

bin_PROGRAMS = \
	Cities3D

### Libraries

pkglib_LTLIBRARIES = \
	libjson_spirit.la \
	libDatabaseCore.la \
	libModelCore.la \
	libRulesCore.la \
	libUICore.la \
	libGLCore.la


### Plugins

pluginsdir = \
	$(pkglibdir)/plugins

plugins_LTLIBRARIES = \
	TexEditor.la \
	CoreRules.la \
	StandardRules.la \
	SeafarersRules.la \
	CitiesRules.la \
	VolcanoRules.la \
	NetworkRules.la

if ENABLE_CITIESONLINE 
bin_PROGRAMS += CitiesOnline
plugins_LTLIBRARIES += CitiesOnlineRules.la
endif

##### Common Defines #####

AM_CXXFLAGS = \
	$(BOOST_CXXFLAGS) \
	$(WX_CXXFLAGS) \
	$(C3D_CXXFLAGS)

AM_LDFLAGS = \
	$(C3D_LDFLAGS)


##### Common Headers #####

nobase_noinst_HEADERS = \
	common/define/defineFile.h \
	common/define/defineGL.h \
	common/define/defineHelper.h \
	common/define/defineStat.h \
	\
	common/stdCommon.h \
	common/wxCommon.h \
	common/boostCommon.h \
	\
	common/style.h


##### Program Modules #####

### Cities3D

Cities3D_CXXFLAGS = \
	-I$(top_srcdir)/src/Cities3D \
	-I$(top_srcdir)/src/GLCore \
	-I$(top_srcdir)/src/UICore \
	-I$(top_srcdir)/src/RulesCore \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	-I$(top_srcdir)/src/common \
	$(AM_CXXFLAGS)
	
Cities3D_SOURCES = \
	Cities3D/Cities3D.bulkbuild.cpp

Cities3D_LDFLAGS = \
	-export-dynamic \
	-no-undefined \
	$(AM_LDFLAGS)

Cities3D_LDADD = \
	libjson_spirit.la \
	libGLCore.la \
	libUICore.la \
	libRulesCore.la \
	libModelCore.la \
	libDatabaseCore.la \
	$(FMOD_LIBS) \
	$(BOOST_LIBS) \
	$(WX_LIBS) \
	$(GLEW_LIBS) \
	$(WX_GL_LIBS)

##### CitiesOnline

if ENABLE_CITIESONLINE

CitiesOnline_CXXFLAGS = \
	-I$(top_srcdir)/src/common/network \
	-I$(top_srcdir)/src/common/encryption \
	$(CRYPTOPP_CXXFLAGS) \
	$(Cities3D_CXXFLAGS)

CitiesOnline_SOURCES = \
	CitiesOnline/CitiesOnline.bulkbuild.cpp

CitiesOnline_LDFLAGS = \
	-export-dynamic \
	-no-undefined \
	$(AM_LDFLAGS)

CitiesOnline_LDADD = \
    libjson_spirit.la \
    libGLCore.la \
    libUICore.la \
    libRulesCore.la \
    libModelCore.la \
    libDatabaseCore.la \
    $(FMOD_LIBS) \
    $(BOOST_LIBS) \
    $(WX_LIBS) \
    $(GLEW_LIBS) \
    $(WX_GL_LIBS) \
	$(CRYPTOPP_LIBS)

else !ENABLE_CITIESONLINE
CitiesOnline_CXXFLAGS=
CitiesOnline_LDADD=
CitiesOnline_LDFLAGS=
CitiesOnline_SOURCES=
endif # ENABLE_CITIESONLINE

##### Core Library Modules #####

CORE_CXXFLAGS = \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/json_spirit \
	$(AM_CXXFLAGS)

CORE_LDFLAGS = \
	-avoid-version \
	$(AM_LDFLAGS)

### JSON Spirit
libjson_spirit_la_CXXFLAGS = \
	$(CORE_CXXFLAGS)

libjson_spirit_la_SOURCES = \
	json_spirit/json_spirit.h \
	json_spirit/json_spirit_reader.cpp \
	json_spirit/json_spirit_reader.h \
	json_spirit/json_spirit_utils.h \
	json_spirit/json_spirit_value.cpp \
	json_spirit/json_spirit_value.h \
	json_spirit/json_spirit_writer.cpp \
	json_spirit/json_spirit_writer.h

libjson_spirit_la_LDFLAGS = \
	$(CORE_LDFLAGS)

### DatabaseCore

libDatabaseCore_la_CXXFLAGS = \
	-DDATABASE_EXPORT \
	-I$(top_srcdir)/src/DatabaseCore \
	$(CORE_CXXFLAGS)

libDatabaseCore_la_SOURCES = \
	DatabaseCore/DatabaseCore.bulkbuild.cpp

libDatabaseCore_la_LDFLAGS = \
	$(CORE_LDFLAGS)


### ModelCore

libModelCore_la_CXXFLAGS = \
	-DMODEL_EXPORT \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	$(CORE_CXXFLAGS)

libModelCore_la_SOURCES = \
	ModelCore/ModelCore.bulkbuild.cpp

libModelCore_la_LDFLAGS = \
	$(CORE_LDFLAGS)


### RulesCore

libRulesCore_la_CXXFLAGS = \
	-DRULES_EXPORT \
	-I$(top_srcdir)/src/RulesCore \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	$(FMOD_CXXFLAGS) \
	$(CORE_CXXFLAGS)

libRulesCore_la_SOURCES = \
	RulesCore/RulesCore.bulkbuild.cpp

libRulesCore_la_LDFLAGS = \
	$(CORE_LDFLAGS)


### UICore

libUICore_la_CXXFLAGS = \
	-DUI_EXPORT \
	-I$(top_srcdir)/src/UICore \
	-I$(top_srcdir)/src/RulesCore \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	$(CORE_CXXFLAGS)

libUICore_la_SOURCES = \
	UICore/UICore.bulkbuild.cpp

libUICore_la_LDFLAGS = \
	$(CORE_LDFLAGS)


### GLCore

libGLCore_la_CXXFLAGS = \
	-DGL_EXPORT \
	-I$(top_srcdir)/src/GLCore \
	-I$(top_srcdir)/src/RulesCore \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	$(GLEW_CXXFLAGS) \
	$(CORE_CXXFLAGS)

libGLCore_la_SOURCES = \
	GLCore/GLCore.bulkbuild.cpp

libGLCore_la_LDFLAGS = \
	$(CORE_LDFLAGS)


##### Rules Library Modules #####

RULES_CXXFLAGS = \
	-I$(top_srcdir)/src/GLCore \
	-I$(top_srcdir)/src/UICore \
	-I$(top_srcdir)/src/RulesCore \
	-I$(top_srcdir)/src/ModelCore \
	-I$(top_srcdir)/src/DatabaseCore \
	-I$(top_srcdir)/src/common \
	-I$(top_srcdir)/src/json_spirit \
	$(AM_CXXFLAGS)

RULES_LDFLAGS = \
	-avoid-version \
	-module \
	$(AM_LDFLAGS)


### TexEditor

TexEditor_la_CXXFLAGS = \
	-I$(top_srcdir)/src/TexEditor \
	$(RULES_CXXFLAGS)

TexEditor_la_SOURCES = \
	common/RuleSetDatabase.cpp \
	TexEditor/TexEditor.bulkbuild.cpp

TexEditor_la_LDFLAGS = \
	$(RULES_LDFLAGS)


### CoreRules

CoreRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/CoreRules \
	$(RULES_CXXFLAGS)

CoreRules_la_SOURCES = \
	common/RuleSetDatabase.cpp \
	CoreRules/CoreRules.bulkbuild.cpp

CoreRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)


### StandardRules

StandardRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/StandardRules \
	$(RULES_CXXFLAGS)

StandardRules_la_SOURCES = \
	common/OptionDatabase.cpp \
	common/RuleSetDatabase.cpp \
	StandardRules/StandardRules.bulkbuild.cpp

StandardRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)


### SeafarersRules

SeafarersRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/SeafarersRules \
	$(RULES_CXXFLAGS)

SeafarersRules_la_SOURCES = \
	common/RuleSetDatabase.cpp \
	common/OptionDatabase.cpp \
	SeafarersRules/SeafarersRules.bulkbuild.cpp

SeafarersRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)

SeafarersRules_la_LIBADD = \
	$(RULES_LIBADD)


### CitiesRules

CitiesRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/CitiesRules \
	$(RULES_CXXFLAGS)

CitiesRules_la_SOURCES = \
	common/OptionDatabase.cpp \
	common/RuleSetDatabase.cpp \
	CitiesRules/CitiesRules.bulkbuild.cpp

CitiesRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)


### VolcanoRules

VolcanoRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/VolcanoRules \
	$(RULES_CXXFLAGS)

VolcanoRules_la_SOURCES = \
	common/OptionDatabase.cpp \
	common/RuleSetDatabase.cpp \
	VolcanoRules/VolcanoRules.bulkbuild.cpp

VolcanoRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)


### NetworkRules

NetworkRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/NetworkRules \
	$(RULES_CXXFLAGS)

NetworkRules_la_SOURCES = \
	common/RuleSetDatabase.cpp \
	NetworkRules/NetworkRules.bulkbuild.cpp

NetworkRules_la_LDFLAGS = \
	$(RULES_LDFLAGS)

### CitiesOnlineRules

if ENABLE_CITIESONLINE

CitiesOnlineRules_la_CXXFLAGS = \
	-I$(top_srcdir)/src/CitiesOnlineRules \
	-I$(top_srcdir)/src/json_spirit \
	$(CRYPTOPP_CXXFLAGS) \
	$(RULES_CXXFLAGS)

CitiesOnlineRules_la_SOURCES = \
	common/RuleSetDatabase.cpp \
	common/encryption/Encryption.cpp \
	CitiesOnlineRules/CitiesOnlineRules.bulkbuild.cpp

CitiesOnlineRules_la_LDFLAGS = \
	$(CRYPTOPP_LIBS) \
	$(RULES_LDFLAGS)

else !ENABLE_CITIESONLINE
CitiesOnlineRules_la_CXXFLAGS=
CitiesOnlineRules_la_SOURCES=
CitiesOnlineRules_la_SOURCES_DIST=
CitiesOnlineRules_la_LDFLAGS=
endif # ENABLE_CITIESONLINE

EXTRA_DIST = \
	common/CppTemplate.txt \
	common/HeaderTemplate.txt \
	common/RuleTemplate.txt \
	common/LogicTemplate.txt \
	\
	Cities3D/cities3d.ico \
	Cities3D/cities3d.rc \
	Cities3D/cities3d.xpm \
	\
	Cities3D/cities3d.vcproj \
	\
	DatabaseCore/DatabaseCore.vcproj \
	ModelCore/ModelCore.vcproj \
	RulesCore/RulesCore.vcproj \
	UICore/UICore.vcproj \
	GLCore/GLCore.vcproj \
	\
	CoreRules/CoreRules.vcproj \
	StandardRules/StandardRules.vcproj \
	SeafarersRules/SeafarersRules.vcproj \
	CitiesRules/CitiesRules.vcproj \
	VolcanoRules/VolcanoRules.vcproj \
	NetworkRules/NetworkRules.vcproj \
	\
	cities3d.sln
