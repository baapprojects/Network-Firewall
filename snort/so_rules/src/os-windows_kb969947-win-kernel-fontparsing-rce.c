/* XXX DOES NOT USE BUILT-IN DETECTION FUNCTION!! XXX
alert tcp $EXTERNAL_NET $HTTP_PORTS -> $HOME_NET any (msg:"WEB-CLIENT Windows TrueType font file parsing interger overflow attempt"; flowbits:isset,file.ttf; flow:to_client,established; content:"|00 01 00 00|"; depth:4; content:"hmtx"; fast_pattern; metadata:policy security-ips drop, service http; reference:cve,2009-2514; reference:url,technet.microsoft.com/en-us/security/bulletin/MS09-065; classtype:attempted-admin; sid:16232; rev:1;)
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

#include "so-util.h"

//#define DEBUG 
#ifdef DEBUG
#define DEBUG_SO(code) code
#else
#define DEBUG_SO(code)
#endif

/* declare detection functions */
int rule16232eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flowbits:isset "file.ttf"; */
static FlowBitsInfo rule16232flowbits0 =
{
    "file.ttf",
    FLOWBIT_ISSET,
    0,
};

static RuleOption rule16232option0 =
{
    OPTION_TYPE_FLOWBIT,
    {
        &rule16232flowbits0
    }
};
/* flow:established, to_client; */
static FlowFlags rule16232flow1 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule16232option1 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule16232flow1
    }
};
// content:"|00 01 00 00|", depth 4; 
static ContentInfo rule16232content2 = 
{
    (uint8_t *) "|00 01 00 00|", /* pattern (now in snort content format) */
    4, /* depth */
    0, /* offset */
    CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16232option2 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16232content2
    }
};
#ifndef CONTENT_FAST_PATTERN
#define CONTENT_FAST_PATTERN 0
#endif
// content:"hmtx", depth 0, fast_pattern; 
static ContentInfo rule16232content3 = 
{
    (uint8_t *) "hmtx", /* pattern (now in snort content format) */
    0, /* depth */
    0, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16232option3 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16232content3
    }
};

/* references for sid 16232 */
/* reference: cve "2009-2514"; */
static RuleReference rule16232ref1 = 
{
    "cve", /* type */
    "2009-2514" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS09-065"; */
static RuleReference rule16232ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS09-065" /* value */
};

/* reference: cve "2010-2741"; */
static RuleReference rule16232ref3 =
{
    "cve", /* type */
    "2010-2741" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS10-078"; */
static RuleReference rule16232ref4 =
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS10-078" /* value */
};

static RuleReference *rule16232refs[] =
{
    &rule16232ref1,
    &rule16232ref2,
    &rule16232ref3,
    &rule16232ref4,
    NULL
};

/* metadata for sid 16232 */
/* metadata:service http, policy security-ips drop; */
//static RuleMetaData rule16232service1 = 
//{
//    "service http"
//};


//static RuleMetaData rule16232policy1 = 
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule16232metadata[] =
{
//    &rule16232service1,
//    &rule16232policy1,
    NULL
};

RuleOption *rule16232options[] =
{
    &rule16232option0,
    &rule16232option1,
    &rule16232option2,
    &rule16232option3,
    NULL
};

Rule rule16232 = {
   
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
       3,  /* genid */
       16232, /* sigid */
       8, /* revision */
   
       "attempted-admin", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "OS-WINDOWS Windows TrueType font file parsing integer overflow attempt",     /* message */
       rule16232refs /* ptr to references */
       ,rule16232metadata
   },
   rule16232options, /* ptr to rule options */
   &rule16232eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule16232eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const uint8_t *beg_of_payload, *end_of_payload = 0;        
    uint16_t i, numTables;
    uint32_t offset, length; 

    DEBUG_SO(const char *name="rule16232eval";)
    DEBUG_SO(printf("%s: enter\n", name);)
    
    if(sp == NULL)        
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
    // flowbits:isset "file.ttf";
    if (processFlowbits(p, rule16232options[0]->option_u.flowBit) <= 0)
        return RULE_NOMATCH;

    // flow:established, to_client;
    if (checkFlow(p, rule16232options[1]->option_u.flowFlags) <= 0)
        return RULE_NOMATCH;
            
    // content:"|00 01 00 00|", depth 4;
    if (contentMatch(p, rule16232options[2]->option_u.content, &cursor_normal) > 0)
    {
        // We don't need to check "hmtx" again since it is fast_pattern
        // content:"hmtx", depth 0, fast_pattern;
        // if (contentMatch(p, rule16232options[3]->option_u.content, &cursor_normal) > 0) {
        //     return RULE_MATCH;

        if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
            return RULE_NOMATCH;
        
        if (cursor_normal + 24 > end_of_payload)
            return RULE_NOMATCH;
            
        numTables = read_big_16(cursor_normal);
        DEBUG_SO(printf("%s: numTables = 0x%04x\n", name, numTables);)

        cursor_normal += 8 + 8;  // 8 bytes = numTables(2), searchRange(2), entrySelector(2), rangeShift(2)
                              // 8 bytes = Tag(4), checkSum(4) in the the first entry
            
        for (i = 0; i < numTables; i++)
        {
            if (cursor_normal + 8 > end_of_payload)
                return RULE_NOMATCH;
            
            offset = read_big_32(cursor_normal);  // read offset(4)
            DEBUG_SO(printf("%s: offset = 0x%08x\n", name, offset);)
            cursor_normal += 4;                   
            length = read_big_32(cursor_normal);  // read length(4)
            DEBUG_SO(printf("%s: length = 0x%08x\n", name, length);)
            
            if (offset + length < offset)      // check integer overwrap
                return  RULE_MATCH;
            
            cursor_normal += 4 + 8;               // skip length(4) and the next entry's Tag(4), checkSum(4)
        }            
    }
    
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule16232,
    NULL
};
*/
