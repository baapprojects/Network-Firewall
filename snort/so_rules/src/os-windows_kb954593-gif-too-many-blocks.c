/* Does not use built-in detection function.
alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT GDI+ GIF image invalid number of extension blocks buffer overflow attempt"; flow:to_client,established; content:"GIF8"; metadata:service http, policy security-ips drop, policy balanced-ips drop; reference:cve,2008-3013; reference:url,technet.microsoft.com/en-us/security/bulletin/MS08-052; classtype:attempted-user; sid:14260; rev:1;)
*/
/*
 * Use at your own risk.
 *
 * Copyright (C) 2005-2008 Sourcefire, Inc.
 * 
 * This file is autogenerated via rules2c, by Brian Caswell <bmc@sourcefire.com>
 */


#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#ifndef EXP2
#define EXP2(A) 1 << A
#endif

//#define DEBUG 1
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif


/* declare detection functions */
int rule14260eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_client; */
static FlowFlags rule14260flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule14260option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule14260flow0
    }
};
// content:"GIF8"; 
static ContentInfo rule14260content1 = 
{
    (uint8_t *) "GIF8", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */ // XXX - need to add CONTENT_FAST_PATTERN support
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule14260option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule14260content1
    }
};

/* references for sid 14260 */
/* reference: cve "2008-3013"; */
static RuleReference rule14260ref1 = 
{
    "cve", /* type */
    "2008-3013" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS08-052"; */
static RuleReference rule14260ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS08-052" /* value */
};

/* reference: bugtraq "31020"; */
static RuleReference rule14260ref3 =
{
    "bugtraq", /* type */
    "31020" /* value */
};


static RuleReference *rule14260refs[] =
{
    &rule14260ref1,
    &rule14260ref2,
    &rule14260ref3,
    NULL
};
/* metadata for sid 14260 */
/* metadata:service http, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule14260service1 = 
{
    "service http"
};


//static RuleMetaData rule14260policy1 = 
//{
//    "policy balanced-ips drop"
//};
//
//static RuleMetaData rule14260policy2 = 
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule14260metadata[] =
{
    &rule14260service1,
//    &rule14260policy1,
//    &rule14260policy2,
    NULL
};
RuleOption *rule14260options[] =
{
    &rule14260option0,
    &rule14260option1,
    NULL
};

Rule rule14260 = {
   
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
       14260, /* sigid */
       10, /* revision */
   
       "attempted-user", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "OS-WINDOWS Microsoft Windows GDI+ GIF image invalid number of extension blocks buffer overflow attempt",     /* message */
       rule14260refs /* ptr to references */
       ,rule14260metadata
   },
   rule14260options, /* ptr to rule options */
   &rule14260eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule14260eval(void *p) {
   const uint8_t *cursor_normal = 0, *beg_of_payload, *end_of_payload;
   SFSnortPacket *sp = (SFSnortPacket *) p;
   const uint8_t *tmp_ptr;

   uint32_t gct_len = 0; // Length of Global Color Table, if it exists
   uint32_t block_count = 0; // Track extension blocks 
   DEBUG_SO(uint32_t block_total = 0;) // Track extension blocks 

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;
    
   // flow:established, to_client;
   if(checkFlow(p, rule14260options[0]->option_u.flowFlags) <= 0 )
      return RULE_NOMATCH;

   // content:"GIF8";
   if(contentMatch(p, rule14260options[1]->option_u.content, &cursor_normal) <= 0)
      return RULE_NOMATCH;

   if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
      return RULE_NOMATCH;

   // At this point, we hopefully are at the start of a GIF and hopefully are
   // sitting with two more bytes remaining in our GIF version header
   if(cursor_normal + 7 >= end_of_payload)
      return RULE_NOMATCH;

   // Check for the rest of GIF89a/GIF87a
   if((*cursor_normal != '9' && *cursor_normal != '7') || (*(cursor_normal + 1) != 'a'))
      return RULE_NOMATCH;

   cursor_normal += 2;

   // Check the GCT flag.  If it's present, find the length of the GCT.
   if(*(cursor_normal + 4) & 0x80) {
      gct_len = 3 * EXP2(((*(cursor_normal + 4) & 0x07) + 1));
   }

   // len of LSD + len of GCT
   tmp_ptr = cursor_normal + gct_len + 7;

   DEBUG_SO(printf("tmp_ptr: %d\n", (int )tmp_ptr);)

   if(tmp_ptr < cursor_normal) // Int overflow
      return RULE_NOMATCH;

   cursor_normal = tmp_ptr;

   // Now get all crazy-like and read the GIF data.
   // 0x3b is EOF marker
   while((cursor_normal + 1 < end_of_payload) && (*cursor_normal == 0x21)) { // process extension blocks
      DEBUG_SO(block_total++;) // Count total blocks if debugging is on

      // 0x21 defines an extension block.  The first byte of an Extension block is an
      // Extension label.  If that label is not one of the GIF 89a defined labels:
      //
      //   * 0xF9 - Graphic Control Extension
      //   * 0xFE - Comment Extension
      //   * 0x01 - Plain Text Extension
      //   * 0xFF - Application Extension
      //

      cursor_normal++; // Skip over Extension tag so we can check the Extension type

      // If it's anything but a defined GIF 89a Extension label, count it.
      if((*cursor_normal != 0x01) && (*cursor_normal != 0xF9) && (*cursor_normal != 0xFE) && (*cursor_normal != 0xFF))
         block_count++;

      cursor_normal++; // Skip over Extension type

      // Data Sub-Blocks are 1-byte size followed by data and
      // are terminated by a subdata block of len 0 (basically a null byte)
      // Check for 0x21 is check for new extension block.  I swear the spec
      // says all blocks must have subdata blocks terminated with a zero-length
      // subdata block, but Office 2003 SP0 crashes with a bunch of records
      // of the form "0x21 0xXX".  Frickin hack.  ><
      while((cursor_normal < end_of_payload) && (*cursor_normal != 0x00) && (*cursor_normal != 0x21))
         cursor_normal += *cursor_normal + 1;  // Add the size value plus size byte to cursor_normal

      if((cursor_normal < end_of_payload) && *cursor_normal == 0x00)
         cursor_normal++; // Skip past null byte 
   }


   // Compare our count:
   //   The vuln is >200, but we're counting extensions and my
   //   WAG says >100 is too many and keeps our data reqs lower
   DEBUG_SO(printf("block_total = %d -> block_count = %d\n", block_total, block_count));
   if(block_count > 100)
      return RULE_MATCH;

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule14260,
    NULL
};
*/
