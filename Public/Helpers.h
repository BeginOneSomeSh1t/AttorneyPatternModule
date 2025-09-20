#pragma once

// Must be used in scope of a class.
#define BEFRIEND_ATTORNEY(AttorneyClass) \
friend class F##AttorneyClass##Attorney;


#define DECLARE_ATTORNEY_BEGIN(Name, ClientType) \
class F##Name##Attorney \
{ \
	\
	using FDecayedClientType = TDecay<ClientType>::Type; \
	using FClientType = FDecayedClientType*;

	/**
	 * An expansion to friend class.
	 * @param PartyClass - class which you want to have access to this attorney's client fields
	 */
#define BEFRIEND_PARTY(PartyClass) \
friend class PartyClass;

#define EXPOSE_CLIENT_FIELD(Field) \
static const decltype(FDecayedClientType::Field)& Get##Field(const FClientType Client) noexcept \
{ \
return Client->Field; \
}\

#define EXPOSE_CLIENT_METHOD(MethodName) \
template<typename... Args> \
static auto MethodName(const FClientType Client, Args&&... Parameters) noexcept -> decltype(Client->MethodName(Forward<Args>(Parameters)...)) \
{ \
using ReturnType = decltype(Client->MethodName(Forward<Args>(Parameters)...)); \
if constexpr (std::is_void_v<ReturnType>) \
{ \
Client->MethodName(Forward<Args>(Parameters)...); \
} \
else \
{ \
return Client->MethodName(Forward<Args>(Parameters)...); \
} \
}\

#define DECLARE_ATTORNEY_END() \
};