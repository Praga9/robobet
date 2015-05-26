bin_PROGRAMS = robobet

robobet_CXXFLAGS = $(CXXFLAGS) $(BOOST_CPPFLAGS)
robobet_LDFLAGS =  $(BOOST_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_SYSTEM_LDFLAGS) $(BOOST_FILESYSTEM_LDFLAGS)  $(BOOST_THREAD_LDFLAGS)
robobet_LIBS =  $(BOOST_PROGRAM_OPTIONS_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_THREAD_LIBS)
robobet_LDADD=  $(BOOST_PROGRAM_OPTIONS_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_THREAD_LIBS)
robobet_SOURCES = main.cpp interval.hpp interval.cpp bet.hpp bet.cpp bet_option.hpp bet_option.cpp more_bet.hpp more_bet.cpp howmany_bet.hpp howmany_bet.cpp when_bet.hpp when_bet.cpp who_bet.hpp who_bet.cpp handicap_bet.hpp handicap_bet.cpp text_resolver.hpp text_resolver.cpp match.hpp match.cpp message_listener.hpp message_listener.cpp bet_xml_parser.hpp bet_xml_parser.cpp message_parser.hpp message_parser.cpp game.hpp game.cpp

AM_YFLAGS=-d
