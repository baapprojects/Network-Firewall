/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>

!!! XXX DOES NOT USE BUILT-IN DETECTION FUNCTIONS XXX !!!

alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Windows BMP image conversion arbitrary code execution attempt"; flow:to_client,established; content:"|0a|BM"; metadata:policy security-ips drop, policy balanced-ips drop; reference:cve,2008-3020; reference:url,technet.microsoft.com/en-us/security/bulletin/ms08-044; classtype:attempted-user; sid:13879; rev:1;)

 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

//#define DEBUG
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif


/* declare detection functions */
int rule13879eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule13879flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule13879option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule13879flow0
    }
};
// content:"|0A|BM"; 
static ContentInfo rule13879content1 = 
{
    (uint8_t *) "BM", /* pattern (now in snort content format) */
    2, /* depth */
    0, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */ // XXX - need to add CONTENT_FAST_PATTERN support
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule13879option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule13879content1
    }
};

// file_data;
static CursorInfo rule13879file_data2 =
{
   0, /* offset */
   CONTENT_BUF_NORMALIZED /* flags */
};

static RuleOption rule13879option2 =
{
#ifndef MISSINGFILEDATA
   OPTION_TYPE_FILE_DATA,
#else
   OPTION_TYPE_SET_CURSOR,
#endif
   {
      &rule13879file_data2
   }
};

/* references for sid 13879 */
/* reference: cve "2008-3020"; */
static RuleReference rule13879ref1 = 
{
    "cve", /* type */
    "2008-3020" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/ms08-044"; */
static RuleReference rule13879ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/ms08-044" /* value */
};

/* reference: cve "2009-2518"; */
static RuleReference rule13879ref3 =
{
    "cve", /* type */
    "2009-2518" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS09-062"; */
static RuleReference rule13879ref4 =
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS09-062" /* value */
};


static RuleReference *rule13879refs[] =
{
    &rule13879ref1,
    &rule13879ref2,
    &rule13879ref3,
    &rule13879ref4,
    NULL
};
/* metadata for sid 13879 */
/* metadata:policy balanced-ips drop, policy security-ips drop; */

static RuleMetaData rule13879service1 =
{
    "service http"
};

//static RuleMetaData rule13879policy1 = 
//{
//    "policy balanced-ips drop"
//};

//static RuleMetaData rule13879policy2 = 
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule13879metadata[] =
{
    &rule13879service1,
//    &rule13879policy1,
//    &rule13879policy2,
    NULL
};
RuleOption *rule13879options[] =
{
    &rule13879option0,
    &rule13879option1,
    &rule13879option2,
    NULL
};

Rule rule13879 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "$HTTP_PORTS", /* SRCPORT   */
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid (HARDCODED!!!) */
       13879, /* sigid */
       12, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "OS-WINDOWS Windows BMP image conversion arbitrary code execution attempt",     /* message */
       rule13879refs /* ptr to references */
       ,rule13879metadata
   },
   rule13879options, /* ptr to rule options */
   &rule13879eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule13879eval(void *p) {
    const uint8_t *beg_of_payload, *cursor_normal = 0, *end_of_payload;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    uint16_t biBitCount;
    uint32_t biClrUsed;

    DEBUG_SO(printf("rule13879eval enter\n"));

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
   // flow:established, to_client;
   if(checkFlow(p, rule13879options[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;   

   // file_data;
   #ifndef MISSINGFILEDATA
   if(fileData(p, rule13879options[2]->option_u.cursor, &cursor_normal) <= 0)
      return RULE_NOMATCH;
   #else
   if(setCursor(p, rule13879options[2]->option_u.cursor, &cursor_normal) <= 0)
      return RULE_NOMATCH;
   #endif

   // content:"BM"; depth:2;
   if(contentMatch(p, rule13879options[1]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   if(cursor_normal + 47 > end_of_payload)
      return RULE_NOMATCH;

   cursor_normal += 26;
   biBitCount = *cursor_normal++;
   biBitCount |= *cursor_normal++ << 8;

   cursor_normal += 16;
   biClrUsed = *cursor_normal++;
   biClrUsed |= *cursor_normal++ << 8;
   biClrUsed |= *cursor_normal++ << 16;
   biClrUsed |= *cursor_normal << 24;

   DEBUG_SO(printf("biBitCount = 0x%02x, biClrUsed = 0x%04x\n", biBitCount, biClrUsed));

   /* Avoid potential wrap and false positive.
    * A shift larger than uint32 will always be greater. */
   /* Also, biBitCount == 0 is a special case, so allow that to pass */
   if (biBitCount >= 32 || biBitCount == 0)
       return RULE_NOMATCH;

   /* Technically, biClrUsed is also a special case, but if it's 0 it can't possibly be
      greater than anything, so we'll just ignore that case */
   if(biClrUsed > (1 << biBitCount))
      return RULE_MATCH;

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule13879,
    NULL
};
*/
