/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 20;       /* snap pixel */
static const int swallowfloating    = 0;
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "mono:size=8", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "mono:size=9";

static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#000000";
static const char col_org[]         = "#fcba03";
static const char col_grayer[]      = "#6b6462";
static const char col_redred[]      = "#ff0101";
static const char normfgcolor[]     = "#bbbbbb";
static const char selfgcolor[]      = "#eeeeee";
static const char col_cyan[]        = "#057070";

/* setting the actual colors */
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]     = { normfgcolor, col_black, col_gray4 },
	[SchemeSel]      = { selfgcolor,  col_black, col_redred},
	[SchemeStatus]   = { col_gray4,     col_black,  "#000000"}, // Statusbar right
	[SchemeTagsSel]  = { col_gray4,   col_black,  "#000000"}, // Tagbar left selected
    [SchemeTagsNorm] = { col_grayer,  col_black,  "#000000"}, // Tagbar left unselected
    [SchemeInfoSel]  = { col_gray3,   col_black,  "#000000"}, // infobar middle  selected
    [SchemeInfoNorm] = { col_gray3,   col_black,  "#000000"}, // infobar middle  unselected
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating      isterminal      noswallow   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,                 0,               0,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,                 0,               0,           -1 },
    { "St",       NULL,       NULL,       0,            0,                 1,               0,           -1 },
    { NULL,       NULL,   "Event Tester", 0,            0,                 0,               1,           -1 },
    
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },               /*  0   first entry is default */
	{ "><>",      NULL },               /*  1   no layout function means floating behavior */
	{ "[M]",      monocle },            /*  2   all windows on top of each other */
    { "[@]",      spiral },             /*  3   floating layout */
    { "[\\]",     dwindle },            /*  4   dwindle layout (decreases in size right and leftward)*/
    { "[D]",      deck },               /*  5   deck layout (stacks several windows on top of each other) */
    { "TTT",      bstack },             /*  6   bstack layout */
    { "===",      bstackhoriz },        /*  7   bstack horiz layout */
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,            		    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_z,	   zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             		    XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // tile
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} }, // floating
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[2]} }, // monocle
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[3]} }, // spiral 
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[4]} }, // dwindle
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[6]} }, // bstack
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[7]} }, // bstackhoriz
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[5]} }, // deck 
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[8]} }, // centered master
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY|ShiftMask,             XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

