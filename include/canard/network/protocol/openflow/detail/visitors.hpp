#ifndef CANARD_NETWORK_OPENFLOW_DETAIL_VISITORS_HPP
#define CANARD_NETWORK_OPENFLOW_DETAIL_VISITORS_HPP

#include <cstdint>
#include <boost/variant/static_visitor.hpp>

namespace canard {
namespace network {
namespace openflow {

    namespace detail {

        template <class Container>
        class encoding_visitor
            : public boost::static_visitor<Container&>
        {
        public:
            encoding_visitor(Container& container)
                : container_{&container}
            {
            }

            template <class T>
            auto operator()(T const& t) const
                -> Container&
            {
                return t.encode(*container_);
            }

        private:
            Container* container_;
        };

        template <class Type>
        class type_visitor
            : public boost::static_visitor<Type>
        {
        public:
            template <class T>
            auto operator()(T const& t) const
                -> Type
            {
                return t.type();
            }
        };

        class length_visitor
            : public boost::static_visitor<std::uint16_t>
        {
        public:
            template <class T>
            auto operator()(T const& t) const
                -> std::uint16_t
            {
                return t.length();
            }
        };

        class property_visitor
            : public boost::static_visitor<std::uint16_t>
        {
        public:
            template <class T>
            auto operator()(T const& t) const
                -> std::uint16_t
            {
                return t.property();
            }
        };

        class equivalent_visitor
            : public boost::static_visitor<bool>
        {
        public:
            template <class T>
            auto operator()(T const& lhs, T const& rhs) const noexcept
                -> bool
            {
                return equivalent(lhs, rhs);
            }

            template <class T, class U>
            auto operator()(T const&, U const&) const noexcept
                -> bool
            {
                return false;
            }
        };

    } // namespace detail

} // namespace openflow
} // namespace network
} // namespace canard

#endif // CANARD_NETWORK_OPENFLOW_DETAIL_VISITORS_HPP
