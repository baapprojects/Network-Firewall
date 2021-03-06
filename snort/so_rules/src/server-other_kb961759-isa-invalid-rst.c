/* XXX DOES NOT USE BUILT-IN DETECTION FUNCTION XXX
 * alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"BAD-TRAFFIC Microsoft ISA Server and Forefront Threat Management Gateway invalid RST denial of service attempt"; flow:to_server,established; metadata:service http, policy security-ips alert, policy balanced-ips alert; reference:cve,2009-0077; reference:url,technet.microsoft.com/en-us/security/bulletin/MS09-016; classtype:attempted-dos; sid:15474;)
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

//#define DEBUG
#ifdef DEBUG
#define DEBUG_WRAP(code) code
#else
#define DEBUG_WRAP(code)
#endif


/* declare detection functions */
int rule15474eval(void *p);

/* declare rule data structures */
/* precompile the stuff that needs pre-compiled */
/* flow:established, to_server; */
static FlowFlags rule15474flow0 = 
{
    FLOW_ESTABLISHED|FLOW_TO_SERVER
};

static RuleOption rule15474option0 =
{
    OPTION_TYPE_FLOWFLAGS,
    {
        &rule15474flow0
    }
};

/* references for sid 15474 */
/* reference: cve "2009-0077"; */
static RuleReference rule15474ref1 = 
{
    "cve", /* type */
    "2009-0077" /* value */
};

/* reference: url "technet.microsoft.com/en-us/security/bulletin/MS09-016"; */
static RuleReference rule15474ref2 = 
{
    "url", /* type */
    "technet.microsoft.com/en-us/security/bulletin/MS09-016" /* value */
};

static RuleReference *rule15474refs[] =
{
    &rule15474ref1,
    &rule15474ref2,
    NULL
};
/* metadata for sid 15474 */
/* metadata:service http, policy balanced-ips alert, policy security-ips alert; */
//static RuleMetaData rule15474service1 = 
//{
//    "service http"
//};


//static RuleMetaData rule15474policy1 = 
//{
//    "policy balanced-ips alert"
//};
//
//static RuleMetaData rule15474policy2 = 
//{
//    "policy security-ips alert"
//};


static RuleMetaData *rule15474metadata[] =
{
//    &rule15474service1,
//    &rule15474policy1,
//    &rule15474policy2,
    NULL
};

RuleOption *rule15474options[] =
{
    &rule15474option0,
    NULL
};

Rule rule15474 = {
   
   /* rule header, akin to => tcp any any -> any any               */{
       IPPROTO_TCP, /* proto */
       "$EXTERNAL_NET", /* SRCIP     */
       "any", /* SRCPORT   */
   
       0, /* DIRECTION */
       "$HOME_NET", /* DSTIP     */
   
       "$HTTP_PORTS", /* DSTPORT   */
   },
   /* metadata */
   { 
       3,  /* genid */
       15474, /* sigid */
       7, /* revision */
   
       "attempted-dos; detection_filter:track by_src, count 50, seconds 10", /* classification */
       0,  /* hardcoded priority XXX NOT PROVIDED BY GRAMMAR YET! */
       "SERVER-OTHER Microsoft ISA Server and Forefront Threat Management Gateway invalid RST denial of service attempt",     /* message */
       rule15474refs /* ptr to references */
       ,rule15474metadata
   },
   rule15474options, /* ptr to rule options */
   &rule15474eval, /* use the built in detection function */
   0 /* am I initialized yet? */
};


/* detection functions */
int rule15474eval(void *p) {
//   const uint8_t *cursor_normal = 0;
   SFSnortPacket *sp = (SFSnortPacket *) p;

   uint32_t tcp_flags;

   DEBUG_WRAP(printf("rule15474eval: enter\n"));

   if(sp == NULL)
      return RULE_NOMATCH;

   if(sp->payload == NULL)
      return RULE_NOMATCH;
 
   if(sp->tcp_header == NULL)
      return RULE_NOMATCH;
 
   // flow:established, to_server;
   if(checkFlow(p, rule15474options[0]->option_u.flowFlags) <= 0 ) 
      return RULE_NOMATCH;

   tcp_flags = sp->tcp_header->flags;

   // Debug output for grins
   DEBUG_WRAP(printf("sp->tcp_header->flags = 0x%08x, TCPHEADER_RST = %d, ", tcp_flags, (tcp_flags & TCPHEADER_RST) ? 1 : 0));
   DEBUG_WRAP(printf("TCPHEADER_ACK = %d\n", (tcp_flags & TCPHEADER_ACK) ? 1 : 0));
   DEBUG_WRAP(printf("sp->tcp_header->acknowledgement = 0x%08x\n", ntohl(sp->tcp_header->acknowledgement)));


   // If RST set AND ACK not set and Acknowledgement not zero, ALERT
   // This rule depends on thresholding to not be completely retarded
   // and false positive everywhere.
   if(!(tcp_flags & TCPHEADER_RST))
      return RULE_NOMATCH;

   if(tcp_flags & TCPHEADER_ACK)
      return RULE_NOMATCH;

   if(sp->tcp_header->acknowledgement) 
      return RULE_MATCH;

   return RULE_NOMATCH;
}

/*
Rule *rules[] = {
    &rule15474,
    NULL
};
*/

