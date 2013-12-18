#ifndef STRUTILTESTH
#define STRUTILTESTH

#include <tut.h>

#include "StrUtil.h"

namespace tut
{
	class StrUtilTestFixture
    {
    };

    typedef test_group<StrUtilTestFixture, 24> StrUtilTestGroup;
	StrUtilTestGroup testGroupStrUtil( "StrUtil tests");
	typedef StrUtilTestGroup::object testStrUtil;

    template<>
    template<>
	void testStrUtil::test<1>()
    {
        set_test_name("'what' ends with 't'");

        ensure( paulst::endsWith( "what", "t" ) );
    }

    template<>
    template<>
	void testStrUtil::test<2>()
    {
        set_test_name("'what' ends with 's'");

        ensure_not( paulst::endsWith( "what", "s" ) );
    }

    template<>
    template<>
	void testStrUtil::test<3>()
    {
        set_test_name("'what' ends with 'what'");

        ensure( paulst::endsWith( "what", "what" ) );
    }

    template<>
    template<>
	void testStrUtil::test<4>()
    {
        set_test_name("'what' ends with 'somewhat'");

        ensure_not( paulst::endsWith( "what", "somewhat" ) );
    }

    template<>
    template<>
	void testStrUtil::test<5>()
    {
        set_test_name("'whatever you say, I will believe it' ends with ' it'");

        ensure( paulst::endsWith( "whatever you say, I will believe it", " it" ) );
    }

    void test_strip_prefix( const char* str, const char* prefixToStrip, const char* expectedResult, bool expectedReturnValue )
    {
        char buffer[1024];
        std::strcpy( buffer, str );
        bool returnValue = paulst::strip_prefix( buffer, prefixToStrip );
        ensure( 0 == std::strcmp( buffer, expectedResult ) );
        ensure( returnValue == expectedReturnValue );

    }

    template<>
    template<>
	void testStrUtil::test<6>()
    {
        set_test_name("strip prefix of 'ab' from 'abcd'");
        test_strip_prefix( "abcd", "ab", "cd", true );
    }

    template<>
    template<>
	void testStrUtil::test<7>()
    {
        set_test_name("strip prefix of 'ab' from 'bcd'");
        test_strip_prefix( "bcd", "ab", "bcd", false );
    }

    template<>
    template<>
	void testStrUtil::test<8>()
    {
        set_test_name("strip prefix of 'abcd' from 'ab'");
        test_strip_prefix( "ab", "abcd", "ab", false );
    }

    template<>
    template<>
	void testStrUtil::test<9>()
    {
        set_test_name("strip prefix of 'ab' from 'ab'");
        test_strip_prefix( "ab", "ab", "", true );
    }

    template<>
    template<>
	void testStrUtil::test<10>()
    {
        set_test_name("strip prefix of 'ab' from ''");
        test_strip_prefix( "", "ab", "", false );
    }

    template<>
    template<>
	void testStrUtil::test<11>()
    {
        set_test_name("format file size: 1024");
        std::string actual = paulst::formatFileSize( 1024 );
        ensure_equals( actual, "1.0 KB" );
    }

    template<>
    template<>
	void testStrUtil::test<12>()
    {
        set_test_name("format file size: 1023");
        std::string actual = paulst::formatFileSize( 1023 );
        ensure_equals( actual, "1023 bytes" );
    }

    template<>
    template<>
	void testStrUtil::test<13>()
    {
        set_test_name("format file size: 1099511627776 (1TB)");
        __int64 size = 1099511627776i64;
        std::string actual = paulst::formatFileSize( size );
        ensure_equals( actual, "1.0 TB" );
    }

    template<>
    template<>
	void testStrUtil::test<14>()
    {
        set_test_name("format file size: 2129171230");
        std::string actual = paulst::formatFileSize( 2129171230 );
        ensure_equals( actual, "2.0 GB" );
    }

    template<>
    template<>
	void testStrUtil::test<15>()
    {
        set_test_name("format file size: 2129171230");
        std::string actual = paulst::formatFileSize( 2129171230 );
        ensure_equals( actual, "2.0 GB" );
    }

    template<>
    template<>
	void testStrUtil::test<16>()
    {
        set_test_name("format file size: 8339717346");
        __int64 size = 8339717346i64;
        std::string actual = paulst::formatFileSize( size );
        ensure_equals( actual, "7.8 GB" );
    }

    template<>
    template<>
	void testStrUtil::test<17>()
    {
        set_test_name("format file size: 5046605076");
        __int64 size = 5046605076i64;
        std::string actual = paulst::formatFileSize( size );
        ensure_equals( actual, "4.7 GB" );
    }

    template<>
    template<>
	void testStrUtil::test<18>()
    {
        set_test_name("finding substrings when there are lots of matches");
        using namespace paulst;
        std::string inputSequence(
"[721&R#f31=[0]&R#f189=[0]&R#f53=[0]&R#f54=[0]&R#f200=[0]&R#f30000=[0]&R#f30002=[0]&R#f30010=[0]&R#f30012=[0]&R#f30020=[0]&R#f30022=[0]&R#f30030=[0]&R#f30032=[0]&R#f30040=[0]&R#f30042=[0]&R#f30050=[0]&R#f30052=[0]&R#f30060=[0]&R#f30062=[0]&R#f30070=[0]&R#f30072=[0]&R#f30080=[0]&R#f30082=[0]&R#f30090=[0]&R#f30092=[0]&R#f30100=[0]&R#f30102=[0]&R#f30110=[0]&R#f30112=[0]&R#f30120=[0]&R#f30122=[0]&R#f30130=[0]&R#f30132=[0]&R#f30140=[0]&R#f30142=[0]&R#f30150=[0]&R#f30152=[0]&R#f30160=[0]&R#f30162=[0]&R#f30170=[0]&R#f30172=[0]&R#f30180=[0]&R#f30182=[0]&R#f30190=[0]&R#f30192=[0]&R#f30200=[0]&R#f30202=[0]&R#f30210=[0]&R#f30212=[0]&R#f30220=[0]&R#f30222=[0]&R#f30230=[0]&R#f30232=[0]&R#f30240=[0]&R#f30242=[0]&R#f30250=[0]&R#f30252=[0]&R#f30260=[0]&R#f30262=[0]&R#f30270=[0]&R#f30272=[0]&R#f30280=[0]&R#f30282=[0]&R#f30290=[0]&R#f30292=[0]&R#f30300=[0]&R#f30302=[0]&R#f23=[0]&R#f42=[0]&R#f3059=[0]&R#f3060=[0]&R#f3061=[0]&R#f3062=[0]&R#f3063=[0]&R#f3064=[0]&R#f3065=[0]&R#f3066=[0]&R#f3088=[0]&R#f3089=[0]&R#f3090=[0]&R#f3132=[0]&R#f3137=[0]&R#f3159=[0]&R#f10691=[0]&R#f10693=[0]&R#f10694=[0]&R#f10695=[0]&R#f10696=[0]&R#f10697=[0]&R#f10711=[0]&R#f10714=[0]&R#f10717=[0]&R#f36=[0]&R#f21003=[0]&R#f37=[0]&R#f93=[0]&R#f94=[0]&R#f95=[0]&R#f96=[0]&R#f102=[0]&R#f4079=[0]&R#f4080=[0]&R#f4081=[0]&R#f4136=[0]&R#f4186=[0]&R#f4189=[0]&R#f4194=[0]&R#f4195=[0]&R#f4196=[0]&R#f4198=[0]&R#f4199=[0]&R#f4200=[0]&R#f4204=[0]&R#f4205=[0]&R#f4207=[0]&R#f21021=[0]&R#f38=[0]&R#f46=[0]&R#f47=[0]&R#f21=[0]&R#f39=[0]&R#f40=[0]&R#f41=[0]&R#f44=[0]&R#f48=[0]&R#f49=[0]&R#f51=[0]&R#f3077=[0]&R#f3160=[0]&R#f20015=[0]&R#f43=[0]&R#f23098=[0]&R#f23099=[0]&R#f23100=[0]&R#f23101=[0]&R#f23102=[0]&R#f23104=[0]&R#f23105=[0]&R#f23106=[0]&R#f23107=[0]&R#f23108=[0]&R#f23109=[0]&R#f23110=[0]&R#f23111=[0]&R#f23112=[0]&R#f23113=[0]&R#f23114=[0]&R#f23115=[0]&R#f23116=[0]&R#f23117=[0]&R#f23118=[0]&R#f23119=[0]&R#f23120=[0]&R#f23121=[0]&R#f23122=[0]&R#f23123=[0]&R#f23124=[0]&R#f23125=[0]&R#f23126=[0]&R#f23127=[0]&R#f23128=[0]&R#f23129=[0]&R#f23130=[0]&R#f19=[0]&R#f45=[0]&R#f77=[0]&R#f78=[0]&R#f145=[0]&R#f3081=[0]&R#f3082=[0]&R#f3083=[0]&R#f3084=[0]&R#f3085=[0]&R#f3086=[0]&R#f3143=[0]&R#f3144=[0]&R#f3146=[0]&R#f3147=[0]&R#f3148=[0]&R#f4092=[0]&R#f4093=[0]&R#f4095=[0]&R#f4096=[0]&R#f4100=[0]&R#f4101=[0]&R#f4103=[0]&R#f4104=[0]&R#f4105=[0]&R#f4106=[0]&R#f4119=[0]&R#f4120=[0]&R#f4122=[0]&R#f4123=[0]&R#f4124=[0]&R#f4125=[0]&R#f4138=[0]&R#f4139=[0]&R#f4140=[0]&R#f4141=[0]&R#f4142=[0]&R#f4143=[0]&R#f4144=[0]&R#f4145=[0]&R#f4146=[0]&R#f4147=[0]&R#f5983=[0]&R#f5984=[0]&R#f5985=[0]&R#f5986=[0]&R#f5987=[0]&R#f5988=[0]&R#f5989=[0]&R#f5990=[0]&R#f5991=[0]&R#f5992=[0]&R#f5993=[0]&R#f6014=[0]&R#f6015=[0]&R#f6016=[0]&R#f6017=[0]&R#f6019=[0]&R#f6020=[0]&R#f6023=[0]&R#f6024=[0]&R#f6032=[0]&R#f6033=[0]&R#f6034=[0]&R#f6038=[0]&R#f6039=[0]&R#f112=[0]&R#f120=[0]&R#f121=[0]&R#f122=[0]&R#f129=[0]&R#f130=[0]&R#f149=[0]&R#f3312=[0]&R#f12720=[0]&R#f670=[0]&R#f680=[0]&R#f699=[0]&R#f709=[0]&R#f728=[0]&R#f738=[0]&R#f6139=[0]&"
"R#f6140=[0]&R#f6141=[0]&R#f10860=[0]&R#f10877=[0]&R#f757=[0]&R#f767=[0]&R#f777=[0]&R#f796=[0]&R#f806=[0]&R#f816=[0]&R#f826=[0]&R#f3426=[0]&R#f6142=[0]&R#f6143=[0]&R#f845=[0]&R#f6138=[0]&R#f10722=[0]&R#f1657=[0]&R#f1667=[0]&R#f3659=[0]&R#f3690=[0]&R#f3819=[0]&R#f5809=[0]&R#f21000=[0]&R#f4674=[0]&R#f6146=[0]&R#f864=[0]&R#f874=[0]&R#f884=[0]&R#f894=[0]&R#f904=[0]&R#f914=[0]&R#f924=[0]&R#f943=[0]&R#f971=[0]&R#f981=[0]&R#f991=[0]&R#f1001=[0]&R#f1011=[0]&R#f1021=[0]&R#f1070=[0]&R#f1080=[0]&R#f1090=[0]&R#f1100=[0]&R#f2624=[0]&R#f2634=[0]&R#f3637=[0]&R#f3647=[0]&R#f6162=[0]&R#f6163=[0]&R#f6164=[0]&R#f10953=[0]&R#f10962=[0]&R#f50=[0]&R#f1249=[0]&R#f1259=[0]&R#f1269=[0]&R#f1279=[0]&R#f2644=[0]&R#f2867=[0]&R#f2877=[0]&R#f2887=[0]&R#f2897=[0]&R#f2907=[0]&R#f2926=[0]&R#f2936=[0]&R#f3436=[0]&R#f3446=[0]&R#f3456=[0]&R#f3466=[0]&R#f3476=[0]&R#f3486=[0]&R#f3496=[0]&R#f3506=[0]&R#f5959=[0]&R#f6157=[0]&R#f6158=[0]&R#f6183=[0]&R#f6194=[0]&R#f10115=[0]&R#f10827=[0]&R#f10895=[0]&R#f1289=[0]&R#f1299=[0]&R#f1309=[0]&R#f1319=[0]&R#f1329=[0]&R#f1339=[0]&R#f1349=[0]&R#f1359=[0]&R#f1369=[0]&R#f1379=[0]&R#f1389=[0]&R#f1408=[0]&R#f1418=[0]&R#f1428=[0]&R#f1438=[0]&R#f1448=[0]&R#f1458=[0]&R#f1468=[0]&R#f1478=[0]&R#f1488=[0]&R#f1498=[0]&R#f1508=[0]&R#f1518=[0]&R#f1528=[0]&R#f1538=[0]&R#f1548=[0]&R#f2654=[0]&R#f3680=[0]&R#f6144=[0]&R#f10767=[0]&R#f10776=[0]&R#f10855=[0]&R#f10912=[0]&R#f1558=[0]&R#f1568=[0]&R#f1578=[0]&R#f1588=[0]&R#f1598=[0]&R#f1608=[0]&R#f1618=[0]&R#f1628=[0]&R#f2664=[0]&R#f3731=[0]&R#f3859=[0]&R#f4407=[0]&R#f4418=[0]&R#f4429=[0]&R#f4440=[0]&R#f4451=[0]&R#f4462=[0]&R#f5364=[0]&R#f10818=[0]&R#f10853=[0]&R#f1050=[0]&R#f1060=[0]&R#f1717=[0]&R#f1727=[0]&R#f1737=[0]&R#f1747=[0]&R#f1757=[0]&R#f2267=[0]&R#f2277=[0]&R#f1647=[0]&R#f1677=[0]&R#f1687=[0]&R#f1697=[0]&R#f1707=[0]&R#f1767=[0]&R#f1777=[0]&R#f1787=[0]&R#f1797=[0]&R#f1807=[0]&R#f1835=[0]&R#f1845=[0]&R#f1873=[0]&R#f1883=[0]&R#f2946=[0]&R#f3526=[0]&R#f3912=[0]&R#f3942=[0]&R#f3972=[0]&R#f3982=[0]&R#f4501=[0]&R#f5057=[0]&R#f6165=[0]&R#f6166=[0]&R#f6167=[0]&R#f6168=[0]&R#f6169=[0]&R#f6170=[0]&R#f6171=[0]&R#f6172=[0]&R#f6173=[0]&R#f6174=[0]&R#f6175=[0]&R#f6176=[0]&R#f10000=[0]&R#f10001=[0]&R#f10002=[0]&R#f10003=[0]&R#f10856=[0]&R#f10857=[0]&R#f10858=[0]&R#f1031=[0]&R#f2110=[0]&R#f6160=[0]&R#f10740=[0]&R#f1920=[0]&R#f1930=[0]&R#f1940=[0]&R#f1950=[0]&R#f1960=[0]&R#f1970=[0]&R#f1980=[0]&R#f1990=[0]&R#f2000=[0]&R#f2010=[0]&R#f2020=[0]&R#f2030=[0]&R#f2040=[0]&R#f2050=[0]&R#f2060=[0]&R#f2070=[0]&R#f2080=[0]&R#f2090=[0]&R#f2100=[0]&R#f4526=[0]&R#f4537=[0]&R#f4548=[0]&R#f4559=[0]&R#f4570=[0]&R#f4581=[0]&R#f4598=[0]&R#f4609=[0]&R#f4620=[0]&R#f4631=[0]&R#f4642=[0]&R#f4653=[0]&R#f5375=[0]&R#f5386=[0]&R#f5663=[0]&R#f5674=[0]&R#f6145=[0]&R#f6156=[0]&R#f10721=[0]&R#f6149=[0]&R#f10006=[0]&R#f2178=[0]&R#f2188=[0]&R#f2296=[0]&R#f2306=[0]&R#f2316=[0]&R#f4717=[0]&R#f4728=[0]&R#f5452=[0]&R#f5463=[0]&R#f5474=[0]&R#f5485=[0]&R#f5496=[0]&R#f10971=[0]&R#f1110=[0]&R#f1120=[0]&R#f1130=[0]&R#f1140=[0]&R#f1150=[0]&R#f2237=[0]&R#f10016="
"[0]&R#f10105=[0]&R#f10749=[0]&R#f10886=[0]&R#f1160=[0]&R#f1170=[0]&R#f1180=[0]&R#f1190=[0]&R#f1200=[0]&R#f1210=[0]&R#f1220=[0]&R#f10758=[0]&R#f1239=[0]&R#f5507=[0]&R#f5518=[0]&R#f5529=[0]&R#f5540=[0]&R#f2956=[0]&R#f3404=[0]&R#f3414=[0]&R#f3571=[0]&R#f3741=[0]&R#f3773=[0]&R#f3799=[0]&R#f4067=[0]&R#f6159=[0]&R#f2335=[0]&R#f3606=[0]&R#f3616=[0]&R#f3751=[0]&R#f2345=[0]&R#f2355=[0]&R#f2365=[0]&R#f3809=[0]&R#f2415=[0]&R#f2844=[0]&R#f2443=[0]&R#f2453=[0]&R#f2463=[0]&R#f2473=[0]&R#f2966=[0]&R#f2976=[0]&R#f2986=[0]&R#f3005=[0]&R#f3627=[0]&R#f3761=[0]&R#f3786=[0]&R#f3894=[0]&R#f3992=[0]&R#f4012=[0]&R#f4022=[0]&R#f4041=[0]&R#f4056=[0]&R#f6150=[0]&R#f6151=[0]&R#f6152=[0]&R#f10844=[0]&R#f2492=[0]&R#f6153=[0]&R#f6154=[0]&R#f6155=[0]&R#f6177=[0]&R#f6179=[0]&R#f10004=[0]&R#f10005=[0]&R#f10007=[0]&R#f10723=[0]&R#f10854=[0]&R#f2247=[0]&R#f2257=[0]&R#f3393=[0]&R#f4792=[0]&R#f4803=[0]&R#f4814=[0]&R#f4825=[0]&R#f4836=[0]&R#f10793=[0]&R#f4287=[0]&R#f4288=[0]&R#f4289=[0]&R#f4290=[0]&R#f4291=[0]&R#f4292=[0]&R#f4293=[0]&R#f4294=[0]&R#f4295=[0]&R#f396=[0]&R#f397=[0]&R#f398=[0]&R#f399=[0]&R#f400=[0]&R#f10133=[0]&R#f10134=[0]&R#f10136=[0]&R#f10137=[0]&R#f10138=[0]&R#f4924=[0]&R#f4935=[0]&R#f4946=[0]&R#f4957=[0]&R#f4968=[0]&R#f4979=[0]&R#f4990=[0]&R#f5001=[0]&R#f5012=[0]&R#f5556=[0]&R#f5699=[0]&R#f5779=[0]&R#f5790=[0]&R#f5866=[0]&R#f20016=[0]&R#f401=[0]&R#f402=[0]&R#f403=[0]&R#f404=[0]&R#f10139=[0]&R#f10140=[0]&R#f10141=[0]&R#f10147=[0]&R#f4250=[0]&R#f4251=[0]&R#f4252=[0]&R#f4253=[0]&R#f4254=[0]&R#f4255=[0]&R#f4256=[0]&R#f4257=[0]&R#f4258=[0]&R#f4259=[0]&R#f4260=[0]&R#f4281=[0]&R#f4282=[0]&R#f4283=[0]&R#f4285=[0]&R#f10142=[0]&R#f10143=[0]&R#f10144=[0]&R#f10145=[0]&R#f10146=[0]&R#f52=[0]&R#f34=[0]&R#f132=[0]&R#f134=[0]&R#f135=[0]&R#f3140=[0]&R#f20001=[0]&R#f20002=[0]&R#f20006=[0]&R#f20007=[0]&R#f20008=[0]&R#f20009=[0]&R#f20012=[0]&R#f20013=[0]&R#f137=[0]&R#f20003=[0]&R#f136=[0]&R#f3079=[0]&R#f20004=[0]&R#f20010=[0]&R#f20011=[0]&R#f20014=[0]&R#f10609=[0]&R#f10610=[0]&R#f10612=[0]&R#f2375=[0]&R#f2385=[0]&R#f2395=[0]&R#f2405=[0]&R#f2674=[0]&R#f2684=[0]&R#f2694=[0]&R#f2704=[0]&R#f2714=[0]&R#f2724=[0]&R#f2734=[0]&R#f2744=[0]&R#f2754=[0]&R#f2764=[0]&R#f2774=[0]&R#f2784=[0]&R#f2794=[0]&R#f2804=[0]&R#f2814=[0]&R#f2824=[0]&R#f2834=[0]&R#f3536=[0]&R#f3546=[0]&R#f3581=[0]&R#f3591=[0]&R#f3700=[0]&R#f3710=[0]&R#f3720=[0]&R#f3829=[0]&R#f3839=[0]&R#f3849=[0]&R#f3872=[0]&R#f3882=[0]&R#f10132=[0]&R#f4229=[0]&R#f4230=[0]&R#f4232=[0]&R#f4233=[0]&R#f4234=[0]&R#f4235=[0]&R#f4236=[0]&R#f4237=[0]&R#f4238=[0]&R#f4239=[0]&R#f4240=[0]&R#f4241=[0]&R#f4242=[0]&R#f4243=[0]&R#f4244=[0]&R#f4245=[0]&R#f4246=[0]&R#f4247=[0]&R#f4248=[0]&R#f4249=[0]&R#f4268=[0]&R#f4269=[0]&R#f4270=[0]&R#f4272=[0]&R#f4275=[0]&R#f4276=[0]&R#f4277=[0]&R#f4279=[0]&R#f4849=[0]&R#f4286=[0]]");
        std::string startPattern("&R#f"), endPattern("=");
        std::vector<std::string> extractedList;
        SubString<std::string> found;
        std::string::const_iterator whereToStart = inputSequence.begin();
        while ( find<std::string>( whereToStart, inputSequence.end(), "&R#f", "=", found ) )
        {
            extractedList.push_back( std::string(found.begin, found.end) );
            whereToStart = found.end;
        }

        ensure_equals( extractedList.size(), 721U );
        ensure_equals( extractedList  [0],   "31" );
        ensure_equals( extractedList[720], "4286" );
    }

    template<>
    template<>
	void testStrUtil::test<19>()
    {
        set_test_name("finding substrings when there are no matches");
        using namespace paulst;
        std::string inputSequence("The quick brown fox jumped over the lazy dog.");
        std::string startPattern("&R#f"), endPattern("=");
        SubString<std::string> found;
        ensure_not( find<std::string>( inputSequence.begin(), inputSequence.end(), "&R#f", "=", found ) );
    }

    template<>
    template<>
	void testStrUtil::test<20>()
    {
        set_test_name("finding substrings when the start-pattern matches but there's no match for the end-pattern");
        using namespace paulst;
        std::string inputSequence("The quick brown fox jumped over the lazy dog.");
        std::string startPattern("brown"), endPattern("=");
        SubString<std::string> found;
        ensure_not( find<std::string>( inputSequence.begin(), inputSequence.end(), "&R#f", "=", found ) );
    }

    template<>
    template<>
	void testStrUtil::test<21>()
    {
        set_test_name("ifind");
        using namespace paulst;
        std::string inputSequence("The quick brown fox jumped over the lazy dog.");
        std::string target("brown");
        ensure    ( "brown", ifind( "brown", inputSequence ) );
        ensure    ( "Brown", ifind( "Brown", inputSequence ) );
        ensure    ( " dog.", ifind( " dog.", inputSequence ) );
        ensure    ( " DOG.", ifind( " DOG.", inputSequence ) );
        ensure_not( " cat.", ifind( " cat.", inputSequence ) );
    }

    
    template<>
    template<>
	void testStrUtil::test<22>()
    {
        set_test_name("count");
        using namespace paulst;

        ensure_equals( count( "abc", "a" ), 1 );
        ensure_equals( count( "aac", "a" ), 2 );
        ensure_equals( count( "aaa", "a" ), 3 );
        ensure_equals( count( "aaa", "b" ), 0 );
        ensure_equals( count( "aaa", "aa" ), 2 );// Unusual?
        ensure_equals( count( "aaa", "aaa" ), 1 );
        ensure_equals( count( "aaa", "aaaa" ), 0 );
    }

    std::string doTrim( const char* str )
    {
        std::string s(str);
        paulst::trim( s );
        return s;
    }

    template<>
    template<>
	void testStrUtil::test<23>()
    {
        set_test_name("trim");
        using namespace paulst;

        ensure_equals( doTrim( "abc" ), "abc" );
    }

    template<>
    template<>
	void testStrUtil::test<24>()
    {
        set_test_name("format");
        using namespace paulst;

        ensure_equals( format( "abc%sdef%dghi", "cat", 34 ), std::string("abccatdef34ghi") );
    }

};
 
#endif

