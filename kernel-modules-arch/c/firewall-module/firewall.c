#include "asm-generic/fcntl.h"
#include "linux/fs.h"
#include "linux/printk.h"
#include <linux/ip.h>
#include <linux/kernel.h> // For KERN_INFO
#include <linux/module.h> // For all Kernal Modules
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Firewall module");
MODULE_AUTHOR("Joona Ylitalo");

static unsigned int nc_hook_fn(void *priv, struct sk_buff *skb,
                               const struct nf_hook_state *state) {
  // firewall logic
  struct iphdr *iph;
  if (!skb)
    return NF_ACCEPT;

  iph = ip_hdr(skb);

  if (iph->protocol == IPPROTO_ICMP) {
    pr_info("packet dropped from: %pI4\n", &iph->saddr);
    return NF_DROP;
  }

  return NF_ACCEPT;
}
static struct nf_hook_ops my_nfho = {
    .hook = nc_hook_fn,
    .hooknum = NF_INET_PRE_ROUTING,
    .pf = PF_INET,
    .priority = NF_IP_PRI_FIRST,
};
// Function called by Hook
// Called when module is loaded using 'insmod'
static int __init firewall_init(void) {
  int ret = nf_register_net_hook(&init_net, &my_nfho);
  if (ret) {
    pr_err("failed to register hook %d\n", ret);
    return ret;
  }
  pr_info("Firewall loaded\n");
  return 0;
}

static void __exit firewall_exit(void) {
  pr_info("Firewall unloaded\n");
  nf_unregister_net_hook(&init_net, &my_nfho);
}

module_init(firewall_init);
module_exit(firewall_exit);
