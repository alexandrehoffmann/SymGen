#ifndef SYMGEN_CRTP_BASE_HPP
#define SYMGEN_CRTP_BASE_HPP

namespace SymGen
{
	
template<class Derived>
class CRTPBase
{
public:
	      Derived& derived()       { return static_cast<      Derived&>(*this); }
	const Derived& derived() const { return static_cast<const Derived&>(*this); }
};
	
} // namespace FSLinalg

#endif // SYMGEN_CRTP_BASE_HPP
