/* 默认秦始皇统一中国那年为第一年，即秦1年。 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "./ccgi-1.1/ccgi.h"

#define MAX_RECORD_SIZE 500
const char tian_gan[][8] = {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};
const char di_zhi[][8]   = {"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};
const char jie_qi[][16]  = {"立春","雨水","惊蛰","春分","清明","谷雨","立夏","小满","芒种","夏至","小暑","大暑","立秋","处暑","白露","秋分","寒露","霜降","立冬","小雪","大雪","冬至","小寒","大寒"};
const char ri[][16] = {"初一","初二","初三","初四","初五","初六","初七","初八","初九","初十",
		       "十一","十二","十三","十四","十五","十六","十七","十八","十九","廿十",
		       "廿一","廿二","廿三","廿四","廿五","廿六","廿七","廿八","廿九","三十","卅一"};
const char yue[][16] = {"正月", "二月", "三月", "四月",
			  "五月", "六月", "七月", "八月",
			  "九月", "十月", "十一月", "腊月"};
const char weekday[][16] = {"日","一","二","三","四","五","六"};
/* 春节年月日 */
const int chun_jie[][3] =  {{2015, 2, 19}, 
			    {2016, 2, 8},
			    {2017, 1, 28},
			    {2018, 16},
			    {2019, 5},
			    {2020, 25}};

const char winter_poem[][100] = {"霜严衣带断 指直不能结", 
				 "朱门酒肉臭 路有冻死骨",
				 "孤舟蓑笠翁 独钓寒江雪",
				 "西风烈 长空雁叫霜晨月",
				 "忽如一夜春风来 千树万树梨花开",
				 "窗含西岭千秋雪",
				 "风雪夜归人",
				 "今我来思 雨雪霏霏",
				 "萧萧北方劲 抚事煎百虑",
				 "北国风光 千里冰封 万里雪飘",
				 "长城内外 惟余茫茫 大河上下 顿失滔滔",
				 "快雪时晴 佳想安善",
				 "李愬风雪入蔡州",
				 "林教头风雪山神庙",
				 "冬 浚洙",
				 "岁末百草零 疾风高冈裂",
				 "岁云暮矣多北风 潇湘洞庭白雪中",
				 "老妻寄异县 十口隔风雪",
				 "梨园子弟散如烟 女乐余姿映寒日",
				 "已是悬崖白丈冰 犹有花枝俏",
				 "驿外断桥边 寂寞开无主",
				 "千山鸟飞绝 万径人踪灭",
				 "风萧萧兮易水寒",
				 "春寒赐浴华清池 温泉水滑洗凝脂",
				 "绿杨烟外晓寒轻",
				 "来如雷霆收震怒 罢如江海凝清光",
				 "乱云低薄暮 急雪舞回风",
				 "横空出世 莽昆仑 阅尽人间春色",
				 "飞起玉龙三百万 搅得周天寒彻",
				 "夜深知雪重",
				 "明月照积雪",
				 "越女新妆出镜心 自知明艳更沉吟",
				 "洞房昨夜停红烛 待晓堂前拜舅姑",
				 "朔气传金柝 寒光照铁衣",
				 "将军百战死 壮士十年归",
				 "寄意寒星荃不察",
				 "惯于长夜过春时",
				 "须臾响急冰弦绝 但见奔星劲有声",
				 "独托幽岩展素心",
				 "华灯照宴敞豪门 娇女严妆侍玉樽",
				 "江山如此多娇",
				 "高丘寂寞竦中夜 芳荃零落无余春",
				 "雪满长安酒价高",
				 "眼空蓄泪泪空垂 暗洒闲抛却为谁",
				 "慈母手中线 游子身上衣",
				 "毫端蕴秀临霜写 口角噙香对月吟",
				 "玉带林中挂 金簪雪里埋",
				 "平林漠漠烟如织 寒山一带伤心碧",
				 "欲洁何曾洁 云空未必空",
				 "朝来寒雨晚来风",
				 "镜里恩情 更那堪梦里功名",
				 "山中高士晶莹雪 寂寞仙姝世外林",
				 "凭栏一片风云气 来作神州挥手人"};

struct gz { /* 天干地支结构，两个编号 */
  int g;
  int z;
}; 

struct form {
  int s;
  int e;
  int detail;
  int display_gl; /* 顺便显示公历 */
};

int get_form_input(struct form *info)
{
	CGI_varlist *varlist;
	CGI_value  *value;
	const char *name;
	int i;
	
	if ((varlist = CGI_get_all(0)) == 0) {
		printf("无数据\r<br>");
		return 0;
	}

	info->s = 1;
	info->e = 1;
	info->detail = 0;
	info->display_gl = 0;

	for (name = CGI_first_name(varlist); name != 0; name = CGI_next_name(varlist)) {

		value = CGI_lookup_all(varlist, 0);

		for (i = 0; value[i] != 0; i++) {
		  /* printf("%s [%d] = %s\r<br>", name, i, value[i]); */
		  if (strcmp(name, "start") == 0)
		    info->s = atoi(value[i]);
		  if (strcmp(name, "end") == 0)
		    info->e = atoi(value[i]);
		  if (strcmp(name, "detail") == 0)
		    info->detail = atoi(value[i]);
		  if (strcmp(name, "gl") == 0)
		    info->display_gl = atoi(value[i]);
		}
	}

	CGI_free_varlist(varlist);  /* free variable list */
	return 0;
}

/* 给定起迄时间，输出干支纪年，秦纪年，公元纪年。flag不等于0则显示干支表。 
   秦始皇统一中国为庚辰年。
 */
void get_gan_zhi(int start, int end, int flag)
{
  int i, j, p, q;
  int t = 6, d = 4;  /* 秦1年的干支编码 */
  if (start < 1)
    start = 1;
  for (i = 1; i <= end; i++) {
    j = i - 222;
    if (j >= 0)
      j = i - 221;
    t = t % 10;
    d = d % 12;
    if (i >= start) {
      printf("%s%s    秦%d年        <font size=-1 color=grey>(%d)</font><br/>\n", tian_gan[t], di_zhi[d], i, j);
      if (flag == 1) {
      for (p = 0; p < 10; p++) {
	if (p != t)
	  printf("%s ", tian_gan[p]);
	else
	  printf("<span class=\"highlite\">%s</span> ", tian_gan[p]);
      }
      printf("<br/>\n");
      for (q = 0; q < 12; q++) {
	if (q != d)
	  printf("%s ", di_zhi[q]);
	else
	  printf("<span class=\"highlite\">%s</span> ", di_zhi[q]);	  
      }
      printf("<br/><br/>\n");
      }
    }
    t++; d++;
  }
}

/* 年干支。 y >= 1。 秦始皇统一六国，y=1。 */
struct gz get_gan_zhi_nian(int y)
{
  /* 秦1年的干支编号未6和4 */
  int tg = (6 + (y - 1)) % 10;
  int dz = (4 + (y - 1)) % 12;
  struct gz x;
  x.g = tg;
  x.z = dz;

  return x;
}

/* 日干支。 */
struct gz get_gan_zhi_ri(struct tm *t)
{
  /* 公元1976年9月9日，毛泽东逝世日甲子 */
  double s;
  int diff;
  struct tm kg = {0};
  struct gz x;

  kg.tm_year = 76; 
  kg.tm_mon = 9 - 1;  
  kg.tm_mday = 9;

  s = difftime(mktime(t), mktime(&kg));

  diff = (int) (s / (24 * 60 * 60));

  x.g = (0 + diff) % 10;
  x.z = (0 + diff) % 12;

  return x;
}

/* 换为时辰 */
int get_shi_chen(int h, int m, int s)
{
  /* 23-1: 子， 1-3: 丑， 。。。 用不到 m 和 s */
  return ((h+1)/2) % 12;
}

/* 立春 */
int li_chun(int month, int mday)
{
  if (month == 2 && mday >= 4 && mday < 19) {
    return mday - 4 + 1;
  }
  return -1;
}

/* 雨水 */
int yu_shui(int month, int mday)
{
  if ((month == 2 && mday >= 19) || (month == 3 && mday < 6)) {
      return (month == 2) ? mday - 19 + 1 : 13 + mday;
  }
  return -1;
}

/* 惊蛰 */
int jing_zhe(int month, int mday)
{
  if (month == 3 && mday >= 6 && mday < 21) {
    return mday - 6 + 1;
  }
  return -1;
}

/* 春分 */
int chun_fen(int month, int mday)
{
  if ((month == 3 && mday >= 21) || (month == 4 && mday < 5)) {
      return (month == 3) ? mday - 21 + 1 : 11 + mday;
  }
  return -1;
}

/* 清明 */
int qing_ming(int month, int mday)
{
  if (month == 4 && mday >= 5 && mday < 20) {
    return mday - 5 + 1;
  }
  return -1;
}

/* 谷雨 */
int gu_yu(int month, int mday)
{
  if ((month == 4 && mday >= 20) || (month == 5 && mday < 6)) {
      return (month == 4) ? mday - 20 + 1 : 11 + mday;
  }
  return -1;
}

/* 立夏 */
int li_xia(int month, int mday)
{
  if (month == 5 && mday >= 6 && mday < 21) {
    return mday - 6 + 1;
  }
  return -1;
}

/* 小满 */
int xiao_man(int month, int mday)
{
  if ((month == 5 && mday >= 21) || (month == 6 && mday < 6)) {
      return (month == 5) ? mday - 21 + 1 : 11 + mday;
  }
  return -1;
}

/* 芒种 */
int mang_zhong(int month, int mday)
{
  if (month == 6 && mday >= 6 && mday < 22) {
    return mday - 6 + 1;
  }
  return -1;
}

/* 夏至 */
int xia_zhi(int month, int mday)
{
  if ((month == 6 && mday >= 22) || (month == 7 && mday < 7)) {
      return (month == 6) ? mday - 22 + 1 : 9 + mday;
  }
  return -1;
}

/* 小暑 */
int xiao_shu(int month, int mday)
{
  if (month == 7 && mday >= 7 && mday < 23) {
    return mday - 7 + 1;
  }
  return -1;
}

/* 大暑 */
int da_shu(int month, int mday)
{
  if ((month == 7 && mday >= 23) || (month == 8 && mday < 8)) {
      return (month == 7) ? mday - 23 + 1 : 9 + mday;
  }
  return -1;
}

/* 立秋 */
int li_qiu(int month, int mday)
{
  if (month == 8 && mday >= 8 && mday < 23) {
    return mday - 8 + 1;
  }
  return -1;
}

/* 处暑 */
int chu_shu(int month, int mday)
{
  if ((month == 8 && mday >= 23) || (month == 9 && mday < 8)) {
      return (month == 8) ? mday - 23 + 1 : 9 + mday;
  }
  return -1;
}

/* 白露 */
int bai_lu(int month, int mday)
{
  if (month == 9 && mday >= 8 && mday < 23) {
    return mday - 8 + 1;
  }
  return -1;
}

/* 秋分 */
int qiu_fen(int month, int mday)
{
  if ((month == 9 && mday >= 23) || (month == 10 && mday < 8)) {
      return (month == 9) ? mday - 23 + 1 : 8 + mday;
  }
  return -1;
}

/* 寒露 */
int han_lu(int month, int mday)
{
  if (month == 10 && mday >= 8 && mday < 23) {
    return mday - 8 + 1;
  }
  return -1;
}

/* 霜降 */
int shuang_jiang(int month, int mday)
{
  if ((month == 10 && mday >= 23) || (month == 11 && mday < 7)) {
      return (month == 10) ? mday - 23 + 1 : 9 + mday;
  }
  return -1;
}

/* 立冬 */
int li_dong(int month, int mday)
{
  if (month == 11 && mday >= 7 && mday < 22) {
    return mday - 7 + 1;
  }
  return -1;
}

/* 小雪 */
int xiao_xue(int month, int mday)
{
  if ((month == 11 && mday >= 22) || (month == 12 && mday < 8)) {
      return (month == 11) ? mday - 22 + 1 : 9 + mday;
  }
  return -1;
}

/* 大雪 */
int da_xue(int month, int mday)
{
  if (month == 12 && mday >= 8 && mday < 22) {
    return mday - 8 + 1;
  }
  return -1;
}

/* 冬至 */
int dong_zhi(int month, int mday)
{
  if ((month == 12 && mday >= 22) || (month == 1 && mday < 5)) {
      return (month == 12) ? mday - 22 + 1 : 10 + mday;
  }
  return -1;
}

/* 小寒 */
int xiao_han(int month, int mday)
{
  if (month == 1 && mday >= 5 && mday < 20) {
    return mday - 5 + 1;
  }
  return -1;
}

/* 大寒 */
int da_han(int month, int mday)
{
  if ((month == 1 && mday >= 20) || (month == 2 && mday < 4)) {
      return (month == 1) ? mday - 20 + 1 : 12 + mday;
  }
  return -1;
}

/* 24节气 */
int get_jie_qi(int month, int mday, int *x)
{
  int offset;

  if ((offset = li_chun(month, mday)) != -1) {
    *x = 0;
    return offset;
  }

  if ((offset = yu_shui(month, mday)) != -1) {
    *x = 1;
    return offset;
  }

  if ((offset = jing_zhe(month, mday)) != -1) {
    *x = 2;
    return offset;
  }

  if ((offset = chun_fen(month, mday)) != -1) {
    *x = 3;
    return offset;
  }

  if ((offset = qing_ming(month, mday)) != -1) {
    *x = 4;
    return offset;
  }

  if ((offset = gu_yu(month, mday)) != -1) {
    *x = 5;
    return offset;
  }

  if ((offset = li_xia(month, mday)) != -1) {
    *x = 6;
    return offset;
  }

  if ((offset = xiao_man(month, mday)) != -1) {
    *x = 7;
    return offset;
  }

  if ((offset = mang_zhong(month, mday)) != -1) {
    *x = 8;
    return offset;
  }

  if ((offset = xia_zhi(month, mday)) != -1) {
    *x = 9;
    return offset;
  }

  if ((offset = xiao_shu(month, mday)) != -1) {
    *x = 10;
    return offset;
  }

  if ((offset = da_shu(month, mday)) != -1) {
    *x = 11;
    return offset;
  }

  if ((offset = li_qiu(month, mday)) != -1) {
    *x = 12;
    return offset;
  }

  if ((offset = chu_shu(month, mday)) != -1) {
    *x = 13;
    return offset;
  }

  if ((offset = bai_lu(month, mday)) != -1) {
    *x = 14;
    return offset;
  }

  if ((offset = qiu_fen(month, mday)) != -1) {
    *x = 15;
    return offset;
  }

  if ((offset = han_lu(month, mday)) != -1) {
    *x = 16;
    return offset;
  }

  if ((offset = shuang_jiang(month, mday)) != -1) {
    *x = 17;
    return offset;
  }

  if ((offset = li_dong(month, mday)) != -1) {
    *x = 18;
    return offset;
  }

  if ((offset = xiao_xue(month, mday)) != -1) {
    *x = 19;
    return offset;
  }

  if ((offset = da_xue(month, mday)) != -1) {
    *x = 20;
    return offset;
  }

  if ((offset = dong_zhi(month, mday)) != -1) {
    *x = 21;
    return offset;
  }

  if ((offset = xiao_han(month, mday)) != -1) {
    *x = 22;
    return offset;
  }

  if ((offset = da_han(month, mday)) != -1) {
    *x = 23;
    return offset;
  }

  return -1;
}

void print_random_poem(int i)
{
  int season;

  srand(time(NULL));
  season = (int) i / 6;
  switch (season) {
  case 0:
    break;
  case 1:
    break;
  case 2:
    break;
  default:
    printf("<br/>%s<br/>", winter_poem[rand() % (sizeof(winter_poem)/sizeof(winter_poem[0]))]);
  }
}

void bottom()
{
  printf("<br/><br/><font size=-1><a href=\"http://zhaoqipengbo.com\">朝气蓬勃社</a>出品</font><br/>");
}

void print_month(int month, int day, int num_days, double r, double cx, double cy, struct form info)
{
  int i;
  double radius = 270;
  double step = 2.0 * M_PI / num_days;
  double x, y;
  double a = M_PI;
  double opacity;

  for (i = 0; i < num_days; i++) {
    opacity = (i + 1 <= 15) ? (i + 1) * 1.0/15 : (num_days - i) * 1.0/15;
    x = cx + radius * cos(a);
    y = cy + radius * sin(a);
    a += step;
    printf("<g>\n");
    if (i + 1 == day) {
      printf("<circle cx=%g cy=%g r=%g fill=\"yellow\" opacity = %g />\n", x, y, r, opacity);
      printf("<text x=%g y=%g fill=\"black\" font-weight=\"bold\">%s</text>\n", x - 15, y + 10, ri[i]);
      printf("<text x=%g y=%g fill=\"black\" font-size=\"10\" font-weight=\"bold\">%s</text>\n", x - 15, y - 10, yue[month-1]);
      if (info.display_gl == 1) {
	time_t t;
	struct tm *s;
	t = time(NULL);
	s = localtime(&t);
	printf("<text x=%g y=%g fill=\"#606060\" font-size=\"8\" font-weight=\"normal\">%02d.%02d %s</text>\n", x - 14, y + 22, s->tm_mon+1, s->tm_mday, weekday[s->tm_wday]);  
      }
    } else if (i + 1 < day) {
      printf("<circle cx=%g cy=%g r=%g fill=\"yellow\" opacity = %g />\n", x, y, r, opacity);
      printf("<text x=%g y=%g>%s</text>\n", x - 15, y + 8, ri[i]);
    } else {
      printf("<circle cx=%g cy=%g r=%g fill=\"none\" stroke=\"#F0F0F0\" />\n", x, y, r);
      printf("<text x=%g y=%g fill=\"grey\">%s</text>\n", x - 15, y + 8, ri[i]);
    }
    printf("</g>\n");
  }
}

void print_jieqi_progress(int curr_jq, int offset, double r, double cx, double cy)
{
  int i;
  double radius = 200.0;
  /* double r = 33.0, cx = 400.0, cy = 300.0; */
  double step = 2 * M_PI / 24.0;
  double x, y;
  double a = M_PI;
  int R, G, B;
  double opacity;
  int season_colors[][4] = {{0x30,0xFF,0x00},{0x70,0xA0,0x00},{0xB8, 0xA0, 0x00}, {0x00, 0xE0, 0xFF}}; /* 红绿蓝三色 */

  R = 255;
  G = 0;
  B = 0;
  opacity = 0.1;

  for (i = 0; i < 24; i++) {
    R = season_colors[(int) i / 6][0];
    G = season_colors[(int) i / 6][1];
    B = season_colors[(int) i / 6][2];
    opacity = (i + 1) % 6 == 0 ? opacity + 0.15 : 0.1;
    printf("<g>\n");
    x = cx + radius * cos(a);
    y = cy + radius * sin(a);
    a += step;
    if (i < curr_jq) {
      printf("<circle cx=%g cy=%g r=%g fill=\"rgba(%d, %d, %d, %g)\" />\n",
	     x, y, r, R, G, B, opacity);

    } else if (i == curr_jq) {
      printf("<defs>\n");
      printf("<radialGradient id=\"g\" x1=\"0%%\" y1=\"0%%\" x2=\"100%%\" y2=\"0%%\" spreadMethod=\"pad\" >\n");
      printf("<stop offset=0%%  stop-color=rgb(%d,%d,%d) stop-opacity = %g />\n", R,G,B, 0.9);
      printf("<stop offset=%g%%  stop-color=rgb(%d,%d,%d) stop-opacity = %g />\n", 100.0 * offset/14, R,G,B, 0.5);
      printf("<stop offset=100%%  stop-color=white stop-opacity = 0.0 />\n");
      printf("</radialGradient>\n");
      printf("</defs>\n");
      printf("<circle cx=%g cy=%g r=%g style=\"fill:url(#g); stroke:black; stroke-width:0;\" />\n", x, y, r);
      /* printf("<circle cx=%g cy=%g r=%g fill=\"rgb(%d, %d, %d)\" stroke=\"black\" stroke-width=\"5\" opacity=%g />\n",
	 x, y, r, R, G, B, opacity); */

    } else {
      printf("<circle cx=%g cy=%g r=%g fill=\"none\" stroke=\"#F0F0F0\" />\n", x, y, r);
    }
    printf("<a xlink:href=\"24jq/%s.html\">\n", jie_qi[i]);
    printf("<text x=%g y=%g>%s</text>\n", x - 14, y + 8, jie_qi[i]);
    /* printf("<text x=%g y=%g fill=\"#909090\">%s</text>\n", x - 14, y + 8, jie_qi[i]); */
    printf("</a>");
  printf("</g>\n");  
  }
}

void print_circles(int curr_jq, int offset, int month, int day, int num_days, double r, double cx, double cy, struct form info)
{
  printf("<svg width=%g height=%g>\n", 2*cx, 2*cy);
  print_jieqi_progress(curr_jq, offset, r, cx, cy);
  print_month(month, day, num_days, r * 0.88, cx, cy, info);
  printf("</svg>\n");
}

void convert_to_qinli_nian_yue_ri(int year, int month, int day, int *qn, int *qy, int *qr, int *n)
{
  int i;
  int m, d;
  /* 公历年月日与农历年月日转化表 */
  const int year2015[][5] = {{1, 1, 11, 11, 29},  /* 1月1日对应农历11月11日（上一年），共29日 */
			     {1, 20, 12, 1, 30}, 
			     {2, 19, 1, 1, 30}, 
			     {3, 20, 2, 1, 30},
			     {4, 18, 3, 1, 30},
			     {5, 18, 4, 1, 29},
			     {6, 16, 5, 1, 29},
			     {7, 16, 6, 1, 30},
			     {8, 14, 7, 1, 29},
			     {9, 13, 8, 1, 30},
			     {10, 13, 9, 1, 30},
			     {11, 12, 10, 1, 30},
			     {12, 11, 11, 1, 29}};
  
  *qn = 0; /* TODO */
  *qy = 1;
  *qr = 1;
  *n = 30;
  for (i = 0; i < sizeof(year2015)/sizeof(year2015[0]) - 1; i++) {
    m = year2015[i][0];
    d = year2015[i][1];
    if ((month == m && day >= d && month < year2015[i+1][0]) || (month == year2015[i+1][0] && day < year2015[i+1][1])) {
      *qy = year2015[i][2]; /* 秦月 */
      *qr = (day - d) + year2015[i][3]; /* 秦日 */
      *n = year2015[i][4];
      return;
    }
  }
  
  if (month == year2015[i][0] && day >= year2015[i][1]) {
      *qy = year2015[i][2]; /* 秦月 */
      *qr = (day - year2015[i][1]) + year2015[i][3]; /* 秦日 */
      *n = year2015[i][4];
  }
}


int main(int argc, char *argv[])
{
  int s, e, flag = 0;
  struct gz curr_year, curr_date;
  time_t t;
  struct tm *time_info;
  int qin_year;
  int shi;
  int jq, offset; 
  struct form info;
  int chun_jie_month, chun_jie_day;
  int qn, qy, qr, ndays;

  get_form_input(&info);

  printf("Content-type: text/html;charset=utf-8\r\n\r\n");
  printf("<!DOCTYPE html>");
  printf("<html lang=\"zh-CN\">");
  printf("<head><meta charset=\"utf-8\"><title>秦历</title><style>.highlite{background:#add8e6;}</style></head>");
  printf("<body>");
  printf("<p>");

  t = time(NULL);
  time_info = localtime(&t);

  if (time_info->tm_year + 1900 - 2015 < sizeof(chun_jie)/sizeof(chun_jie[0])) {
    chun_jie_month = chun_jie[time_info->tm_year + 1900 - 2015][1];
    chun_jie_day = chun_jie[time_info->tm_year + 1900 - 2015][2];
    qin_year = ((time_info->tm_mon + 1 == chun_jie_month && time_info->tm_mday >= chun_jie_day) || time_info->tm_mon + 1 > chun_jie_month) ? (time_info->tm_year + 1900 + 221) : (time_info->tm_year - 1 + 1900 + 221);
  } else { /* 不知道哪天新年，暂用公历年 */
    qin_year = time_info->tm_year + 1900 + 221;
  }
  curr_year = get_gan_zhi_nian(qin_year);
  curr_date = get_gan_zhi_ri(localtime(&t));
  shi = get_shi_chen(time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
  offset = get_jie_qi(time_info->tm_mon + 1, time_info->tm_mday, &jq);

  if (info.s == 0 && info.e == 0) {
    printf("秦%d年 岁在%s%s [%s第%d天] %s%s日 %s时[%02d:%02d:%02d]<br>\n", 
	   qin_year, tian_gan[curr_year.g], di_zhi[curr_year.z], jie_qi[jq], offset,
	   tian_gan[curr_date.g], di_zhi[curr_date.z],
	   di_zhi[shi], time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
    print_random_poem(jq);
    convert_to_qinli_nian_yue_ri(time_info->tm_year + 1900, time_info->tm_mon + 1, time_info->tm_mday, &qn, &qy, &qr, &ndays);
    print_circles(jq, offset, qy, qr, ndays, 33.0, 400.0, 300.0, info);
  } else if (info.s <= 0 || info.e <= 0 || info.s > info.e) {
    printf("<font color=red>输入正确起迄时间</font><br/>\n");
  } else {

  s = info.s;
  e = info.e;
  if (info.detail == 1) {
    flag = 1;
  }

  if ((e - s) > MAX_RECORD_SIZE)
    e = s + MAX_RECORD_SIZE;
  get_gan_zhi(s, e, flag);
  }
  printf("</p>");
  bottom();
  printf("</body>");
  printf("</html>");

  return 0;
}
/* 朝气蓬勃社 (zhaoqipengbo.com) 出品。作者：宣平人， 等等等。。。 */