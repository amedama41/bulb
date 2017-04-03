#ifndef CANARD_NET_OFP_DETAIL_VISITORS_HPP
#define CANARD_NET_OFP_DETAIL_VISITORS_HPP

#include <cstdint>
#include <memory>
#include <boost/variant/static_visitor.hpp>

namespace canard {
namespace net {
namespace ofp {
namespace detail {

  template <class Container>
  class encoding_visitor
    : public boost::static_visitor<Container&>
  {
  public:
    explicit encoding_visitor(Container& container) noexcept
      : container_{std::addressof(container)}
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

  template <class Validator>
  class validation_visitor
    : public boost::static_visitor<void>
  {
  public:
    explicit validation_visitor(Validator validator)
      : validator_(validator)
    {
    }

    template <class T>
    void operator()(T const& t) const
    {
      validator_(t);
    }

  private:
    Validator validator_;
  };

  template <class Type>
  class type_visitor
    : public boost::static_visitor<Type>
  {
  public:
    template <class T>
    auto operator()(T const& t) const noexcept
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
    auto operator()(T const& t) const noexcept
      -> std::uint16_t
    {
      return t.length();
    }
  };

  class byte_length_visitor
    : public boost::static_visitor<std::uint16_t>
  {
  public:
    template <class T>
    auto operator()(T const& t) const noexcept
      -> std::uint16_t
    {
      return t.byte_length();
    }
  };

  class equivalent_visitor
    : public boost::static_visitor<bool>
  {
  public:
    template <class T>
    auto operator()(T const& lhs, T const& rhs) const
        noexcept(noexcept(equivalent(lhs, rhs)))
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

#define DEFINE_OXM_VISITOR(FUNC_NAME, RETURN_TYPE) \
  class FUNC_NAME ## _visitor \
    : public boost::static_visitor<RETURN_TYPE> \
  { \
  public: \
    template <class T> \
    auto operator()(T const& t) const noexcept \
      -> RETURN_TYPE \
    { \
      return t.FUNC_NAME(); \
    } \
  }; \

  DEFINE_OXM_VISITOR(oxm_class, std::uint16_t)
  DEFINE_OXM_VISITOR(oxm_field, std::uint8_t)
  DEFINE_OXM_VISITOR(oxm_type, std::uint32_t)
  DEFINE_OXM_VISITOR(oxm_header, std::uint32_t)
  DEFINE_OXM_VISITOR(oxm_hasmask, bool)
  DEFINE_OXM_VISITOR(oxm_length, std::uint8_t)
  DEFINE_OXM_VISITOR(is_wildcard, bool)
  DEFINE_OXM_VISITOR(is_exact, bool)

#undef DEFINE_OXM_VISITOR

} // namespace detail
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_DETAIL_VISITORS_HPP
