/* Icinga 2 | (c) 2012 Icinga GmbH | GPLv2+ */

#ifndef SERVICE_H
#define SERVICE_H

#include "config/applyrule.hpp"
#include "icinga/i2-icinga.hpp"
#include "icinga/service-ti.hpp"
#include "icinga/macroresolver.hpp"
#include "icinga/host.hpp"
#include <tuple>

using std::tie;

namespace icinga
{

/**
 * An Icinga service.
 *
 * @ingroup icinga
 */
class Service final : public ObjectImpl<Service>, public MacroResolver
{
public:
	DECLARE_OBJECT(Service);
	DECLARE_OBJECTNAME(Service);

	static Service::Ptr GetByNamePair(const String& hostName, const String& serviceName);

	Host::Ptr GetHost() const override;
	int GetSeverity() const override;
	bool GetHandled() const override;

	bool ResolveMacro(const String& macro, const CheckResult::Ptr& cr, Value *result) const override;

	bool IsStateOK(ServiceState state) const override;
	void SaveLastState(ServiceState state, double timestamp) override;

	static ServiceState StateFromString(const String& state);
	static String StateToString(ServiceState state);

	static StateType StateTypeFromString(const String& state);
	static String StateTypeToString(StateType state);

	static void EvaluateApplyRules(const Host::Ptr& host, TimeSpentOnApplyMismatches& timeSpentOnApplyMismatches);

	void OnAllConfigLoaded() override;

	static boost::signals2::signal<void (const Service::Ptr&, const CheckResult::Ptr&, const MessageOrigin::Ptr&)> OnHostProblemChanged;

protected:
	void CreateChildObjects(const Type::Ptr& childType, TimeSpentOnApplyMismatches& timeSpentOnApplyMismatches) override;

	Dictionary::Ptr MakeLocalsForApply() override;

private:
	Host::Ptr m_Host;

	static bool EvaluateApplyRuleInstance(const Host::Ptr& host, const String& name, ScriptFrame& frame, const ApplyRule& rule, bool skipFilter);

	static bool EvaluateApplyRule(
		const Host::Ptr& host, const ApplyRule::Ptr& rule,
		TimeSpentOnApplyMismatches& timeSpentOnApplyMismatches, bool skipFilter = false
	);
};

std::pair<Host::Ptr, Service::Ptr> GetHostService(const Checkable::Ptr& checkable);

}

#endif /* SERVICE_H */
