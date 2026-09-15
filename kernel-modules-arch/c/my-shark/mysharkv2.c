
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/slab.h>
#include <linux/icmp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JoonaY");
MODULE_DESCRIPTION("Simple Linux device driver (Real Linux Device Driver)");
MODULE_VERSION("1.0");

static struct nf_hook_ops *nfho = NULL;

static unsigned int hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	struct iphdr *iph;
  struct icmphdr *icmph;

  if(!skb)
    return NF_ACCEPT;
  iph = ip_hdr(skb);
  if(!iph || iph->protocol != IPPROTO_ICMP)
    return NF_ACCEPT;
  
  icmph = icmp_hdr(skb);
  if(!icmph)
    return NF_ACCEPT;
  if(icmph->type == ICMP_ECHO){
    pr_info("[tcp_intercept] Ping: %pI4 -> %pI4 | id=%u seq=%u\n", &iph->saddr, &iph->daddr, ntohs(icmph->un.echo.id),ntohs(icmph->un.echo.sequence));
  }

  return NF_ACCEPT;
}
// Entry, exit
static int __init my_init(void){
  nfho = kzalloc(sizeof(struct nf_hook_ops), GFP_KERNEL);
  if(!nfho){
    pr_err("Couldnt load memory for nfho, exiting.");
    return -ENOMEM;
  }
	nfho->hook 	= (nf_hookfn*)hfunc;		/* hook function */
	nfho->hooknum 	= NF_INET_PRE_ROUTING;		/* received packets */
	nfho->pf 	= PF_INET;			/* IPv4 */
	nfho->priority 	= NF_IP_PRI_FIRST;		/* max hook priority */	

  int ret = nf_register_net_hook(&init_net, nfho);
  if(ret) {
    pr_err("[tcp_intercept] Failed to register hook %d\n", ret);
    kfree(nfho);
    return ret;
  }
  pr_info("[n. tcp_intercept] ICMP hook initialized.");
  return 0;
}
static void __exit my_exit(void){
	nf_unregister_net_hook(&init_net, nfho);
	kfree(nfho);
  pr_info("[n. tcp_intercept] ICMP hook removed.");
}

module_init(my_init);
module_exit(my_exit);

