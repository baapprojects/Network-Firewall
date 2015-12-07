/*
 * alert tcp $EXTERNAL_NET 445 -> $HOME_NET any (msg:"NETBIOS SMB client TRANS response ring0 remote code execution attempt"; flow:to_client,established; content:"|ff|SMB|25|"; offset:4; depth:5; isdataat:28,relative; metadata:policy balanced-ips drop, policy security-ips drop, service netbios-ssn; classtype:attempted-admin; reference:cve,2010-0476; reference:url,technet.microsoft.com/en-us/security/bulletin/MS10-020; sid:16532;)
*/
/*
 * This code was automatically generated by the Sourcefire VRT Rules2C web generator
 *
 * We make no claims to the correctness or fitness of the code generated or of modifications
 * to the generated code.
 *
 * Use at your own risk.
 *
 * Please report any problems with this tool to research@sourcefire.com
 */

#include "sf_snort_plugin_api.h"
#include "sf_snort_packet.h"

#include "so-util.h"

/* declare detection functions */
int rule16532eval(void *p);

/* declare rule data structures */
/* flow:established, to_client; */
static FlowFlags rule16532flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_CLIENT
};

static RuleOption rule16532option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule16532flow0
    }
};
#ifndef CONTENT_FAST_PATTERN
#define CONTENT_FAST_PATTERN 0
#endif
// content:"|FF|SMB%", payload raw, offset 4, depth 5, fast_pattern; 
static ContentInfo rule16532content1 = 
{
    (uint8_t *) "|FF|SMB%", /* pattern (now in snort content format) */
    5, /* depth */
    4, /* offset */
    CONTENT_FAST_PATTERN|CONTENT_BUF_NORMALIZED, /* flags */
    NULL, /* holder for boyer/moore PTR */
    NULL, /* more holder info - byteform */
    0, /* byteform length */
    0 /* increment length*/
};

static RuleOption rule16532option1 = 
{
    OPTION_TYPE_CONTENT,
    {
        &rule16532content1
    }
};
// isdataat:offset 25, relative, payload raw;
static CursorInfo rule16532isdataat2 =
{
    28, /* offset */
    CONTENT_RELATIVE|CONTENT_BUF_NORMALIZED /* flags */
};

static RuleOption rule16532option2 =
{
    OPTION_TYPE_CURSOR,
    {
        &rule16532isdataat2
    }
};

/* references for sid 16532 */
/* reference: cve "2010-0476"; */
static RuleReference rule16532ref1 = 
{
    "cve", /* type */
    "2010-0476" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS10-020"; */
static RuleReference rule16532ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS10-020" /* value */
};

static RuleReference *rule16532refs[] =
{
    &rule16532ref1,
    &rule16532ref2,
    NULL
};
/* metadata for sid 16532 */
/* metadata:service netbios-ssn, policy balanced-ips drop, policy security-ips drop; */
static RuleMetaData rule16532service1 = 
{
    "service netbios-ssn"
};

//static RuleMetaData rule16532policy1 =
//{
//    "policy security-ips drop"
//};


static RuleMetaData *rule16532metadata[] =
{
    &rule16532service1,
//    &rule16532policy1,
    NULL
};

RuleOption *rule16532options[] =
{
    &rule16532option0,
    &rule16532option1,
    &rule16532option2,
    NULL
};

Rule rule16532 = {
   /* rule header, akin to => tcp any any -> any any */
   {
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "445", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "any", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       16532, /* sigid */
       5, /* revision */
       "attempted-admin", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "NETBIOS SMB client TRANS response ring0 remote code execution attempt",     /* message */
       rule16532refs /* ptr to references */
       ,rule16532metadata
   },
   rule16532options, /* ptr to rule options */
   &rule16532eval, /* replace with NULL to use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule16532eval(void *p) {
    const uint8_t *cursor_normal = 0;
    SFSnortPacket *sp = (SFSnortPacket *) p;

    const uint8_t *beg_of_payload, *end_of_payload;
    uint32_t length = 0;
    uint16_t wc = 0;
    uint16_t bcc;

    if(sp == NULL)
        return RULE_NOMATCH;

    if(sp->payload == NULL)
        return RULE_NOMATCH;
    
    // flow:established, to_client;
    if (checkFlow(p, rule16532options[0]->option_u.flowFlags) > 0 ) {
        // content:"|FF|SMB%", payload raw, offset 4, depth 5, fast_pattern;
        if (contentMatch(p, rule16532options[1]->option_u.content, &cursor_normal) > 0) {
            // isdataat:offset 28, relative, payload raw;
            if (checkCursor(p, rule16532options[2]->option_u.cursor, cursor_normal) > 0) {

                if(getBuffer(sp, CONTENT_BUF_NORMALIZED, &beg_of_payload, &end_of_payload) <= 0)
                     return RULE_NOMATCH;

                //Ensure message type is 0
                if (*(cursor_normal-9) != 0)
                    return RULE_NOMATCH;

                //Extract netbios length
                length = *(cursor_normal-8) << 16;
                length |= *(cursor_normal-7) << 8;
                length |= *(cursor_normal-6);

                //Already know isdataat 28 relative
                cursor_normal += 27;

                //Extract word count
                wc = *cursor_normal << 1; // Someone's being clever multiplying by 2

                //Make sure the word count field, word count data and BCC field are there
                if (cursor_normal + wc + 3 > end_of_payload)
                    return RULE_NOMATCH;

                //Jump over word count field and number of words
                cursor_normal += wc + 1;

                //Extract BCC
                bcc = read_little_16(cursor_normal);
               
                //Alert if BCC is greater than the netbios length - WC*2 - SMB header - WC field - BCC field
                if (bcc > (length - wc - 3 - 32))
                    return RULE_MATCH;
            }
        }
    }
    return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule16532,
    NULL
};
*/
