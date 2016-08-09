#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_CONFIG_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_CONFIG_HPP

#if !defined(CANARD_NET_OFP_HEADER_ONLY)
# if !defined(CANARD_NET_OFP_SEPARATE_COMPILATION)
#  if !defined(CANARD_NET_OFP_USE_EXPLICIT_INSTANTIATION)
#   define CANARD_NET_OFP_HEADER_ONLY 1
#  endif
# endif
#endif

#if defined(CANARD_NET_OFP_HEADER_ONLY)
# define CANARD_NET_OFP_DECL inline
#else
# define CANARD_NET_OFP_DECL
#endif

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_CONFIG_HPP
