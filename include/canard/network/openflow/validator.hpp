#ifndef CANARD_NET_OFP_VALIDATION_VALIDATOR_HPP
#define CANARD_NET_OFP_VALIDATION_VALIDATOR_HPP

#include <utility>

namespace canard {
namespace net {
namespace ofp {
namespace validation {

  struct null_validator
  {
    template <class T>
    void operator()(T const&) const noexcept
    {
    }
  };

  struct default_validator
  {
    template <class T>
    void operator()(T const& t) const
    {
      t.validate(null_validator{});
    }
  };

  struct recursive_validator
  {
    template <class T>
    void operator()(T const& t) const
    {
      t.validate(*this);
    }
  };

  template <class T, class Validator = default_validator>
  auto validate(T&& t, Validator validator = Validator{})
    -> T&&
  {
    validator(t);
    return std::forward<T>(t);
  }

} // namespace validation
} // namespace ofp
} // namespace net
} // namespace canard

#endif // CANARD_NET_OFP_VALIDATION_VALIDATOR_HPP
