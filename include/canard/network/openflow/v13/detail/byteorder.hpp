#ifndef CANARD_NET_OFP_V13_BYTEORDER_CONVERSION_HPP
#define CANARD_NET_OFP_V13_BYTEORDER_CONVERSION_HPP

#include <boost/endian/conversion.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <canard/network/openflow/v13/detail/fusion_adaptor.hpp>
#include <canard/network/openflow/v13/openflow.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace v13 {

    namespace v13_detail {

        struct generic_endian_reverse_inplace
        {
            template <class Field>
            void operator()(Field& field) const
            {
                using boost::endian::endian_reverse_inplace;
                endian_reverse_inplace(field);
            }

            template <class T, std::size_t N>
            void operator()(T (&field)[N]) const
            {
                using boost::endian::endian_reverse_inplace;
                for (T& e : field) { endian_reverse_inplace(e); }
            }

            template <std::size_t N>
            void operator()(char (&)[N]) const {}

            template <std::size_t N>
            void operator()(std::uint8_t (&)[N]) const {}
        };

        inline void endian_reverse_inplace(ofp_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_header data) noexcept
            -> ofp_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_hello_elem_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_hello_elem_header data) noexcept
            -> ofp_hello_elem_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_hello_elem_versionbitmap& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_hello_elem_versionbitmap data) noexcept
            -> ofp_hello_elem_versionbitmap
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_hello& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_hello data) noexcept
            -> ofp_hello
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_switch_config& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_switch_config data) noexcept
            -> ofp_switch_config
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_mod data) noexcept
            -> ofp_table_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port data) noexcept
            -> ofp_port
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_switch_features& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_switch_features data) noexcept
            -> ofp_switch_features
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_status& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_status data) noexcept
            -> ofp_port_status
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_mod data) noexcept
            -> ofp_port_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_match& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_match data) noexcept
            -> ofp_match
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_oxm_experimenter_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_oxm_experimenter_header data) noexcept
            -> ofp_oxm_experimenter_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_header data) noexcept
            -> ofp_action_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_output& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_output data) noexcept
            -> ofp_action_output
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_mpls_ttl& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_mpls_ttl data) noexcept
            -> ofp_action_mpls_ttl
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_push& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_push data) noexcept
            -> ofp_action_push
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_pop_mpls& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_pop_mpls data) noexcept
            -> ofp_action_pop_mpls
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_group& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_group data) noexcept
            -> ofp_action_group
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_nw_ttl& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_nw_ttl data) noexcept
            -> ofp_action_nw_ttl
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_set_field& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_set_field data) noexcept
            -> ofp_action_set_field
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_experimenter_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_experimenter_header data) noexcept
            -> ofp_action_experimenter_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction data) noexcept
            -> ofp_instruction
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction_goto_table& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction_goto_table data) noexcept
            -> ofp_instruction_goto_table
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction_write_metadata& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction_write_metadata data) noexcept
            -> ofp_instruction_write_metadata
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction_actions& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction_actions data) noexcept
            -> ofp_instruction_actions
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction_meter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction_meter data) noexcept
            -> ofp_instruction_meter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_instruction_experimenter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_instruction_experimenter data) noexcept
            -> ofp_instruction_experimenter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_mod data) noexcept
            -> ofp_flow_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_bucket& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_bucket data) noexcept
            -> ofp_bucket
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_group_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_group_mod data) noexcept
            -> ofp_group_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_out& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_out data) noexcept
            -> ofp_packet_out
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_in& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_in data) noexcept
            -> ofp_packet_in
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_removed& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_removed data) noexcept
            -> ofp_flow_removed
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_band_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_band_header data) noexcept
            -> ofp_meter_band_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_band_drop& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_band_drop data) noexcept
            -> ofp_meter_band_drop
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_band_dscp_remark& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_band_dscp_remark data) noexcept
            -> ofp_meter_band_dscp_remark
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_band_experimenter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_band_experimenter data) noexcept
            -> ofp_meter_band_experimenter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_mod& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_mod data) noexcept
            -> ofp_meter_mod
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_error_msg& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_error_msg data) noexcept
            -> ofp_error_msg
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_error_experimenter_msg& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_error_experimenter_msg data) noexcept
            -> ofp_error_experimenter_msg
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_multipart_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_multipart_request data) noexcept
            -> ofp_multipart_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_multipart_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_multipart_reply data) noexcept
            -> ofp_multipart_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_desc& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_desc data) noexcept
            -> ofp_desc
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_stats_request data) noexcept
            -> ofp_flow_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_flow_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_flow_stats data) noexcept
            -> ofp_flow_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_aggregate_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_aggregate_stats_request data) noexcept
            -> ofp_aggregate_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_aggregate_stats_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_aggregate_stats_reply data) noexcept
            -> ofp_aggregate_stats_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_header data) noexcept
            -> ofp_table_feature_prop_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_instructions& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_instructions data) noexcept
            -> ofp_table_feature_prop_instructions
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_next_tables& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_next_tables data) noexcept
            -> ofp_table_feature_prop_next_tables
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_actions& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_actions data) noexcept
            -> ofp_table_feature_prop_actions
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_oxm& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_oxm data) noexcept
            -> ofp_table_feature_prop_oxm
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_feature_prop_experimenter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_feature_prop_experimenter data) noexcept
            -> ofp_table_feature_prop_experimenter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_features& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_features data) noexcept
            -> ofp_table_features
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_table_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_table_stats data) noexcept
            -> ofp_table_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_stats_request data) noexcept
            -> ofp_port_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_port_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_port_stats data) noexcept
            -> ofp_port_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_group_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_group_stats_request data) noexcept
            -> ofp_group_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_bucket_counter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_bucket_counter data) noexcept
            -> ofp_bucket_counter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_group_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_group_stats data) noexcept
            -> ofp_group_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_group_desc& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_group_desc data) noexcept
            -> ofp_group_desc
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_group_features& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_group_features data) noexcept
            -> ofp_group_features
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_multipart_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_multipart_request data) noexcept
            -> ofp_meter_multipart_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_band_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_band_stats data) noexcept
            -> ofp_meter_band_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_stats data) noexcept
            -> ofp_meter_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_config& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_config data) noexcept
            -> ofp_meter_config
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_meter_features& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_meter_features data) noexcept
            -> ofp_meter_features
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_experimenter_multipart_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_experimenter_multipart_header data) noexcept
            -> ofp_experimenter_multipart_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_experimenter_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_experimenter_header data) noexcept
            -> ofp_experimenter_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_header& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_header data) noexcept
            -> ofp_queue_prop_header
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_min_rate& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_min_rate data) noexcept
            -> ofp_queue_prop_min_rate
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_max_rate& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_max_rate data) noexcept
            -> ofp_queue_prop_max_rate
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_prop_experimenter& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_prop_experimenter data) noexcept
            -> ofp_queue_prop_experimenter
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_packet_queue& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_packet_queue data) noexcept
            -> ofp_packet_queue
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_get_config_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_get_config_request data) noexcept
            -> ofp_queue_get_config_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_get_config_reply& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_get_config_reply data) noexcept
            -> ofp_queue_get_config_reply
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_action_set_queue& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_action_set_queue data) noexcept
            -> ofp_action_set_queue
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_stats_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_stats_request data) noexcept
            -> ofp_queue_stats_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_queue_stats& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_queue_stats data) noexcept
            -> ofp_queue_stats
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_role_request& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_role_request data) noexcept
            -> ofp_role_request
        {
            endian_reverse_inplace(data);
            return data;
        }

        inline void endian_reverse_inplace(ofp_async_config& data) noexcept
        {
            boost::fusion::for_each(data, generic_endian_reverse_inplace{});
        }

        inline auto endian_reverse(ofp_async_config data) noexcept
            -> ofp_async_config
        {
            endian_reverse_inplace(data);
            return data;
        }

    } // namespace v13_detail

} // namespace v13
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_V13_BYTEORDER_CONVERSION_HPP
