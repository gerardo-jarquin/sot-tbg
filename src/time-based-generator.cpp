#include <boost/bind.hpp>

#include <dynamic-graph/factory.h>

#include "time-based-generator.hh"

namespace dynamicgraph
{
  namespace sot
  {
    TimeBasedGeneratorGain::TimeBasedGeneratorGain
    (const std::string& name)
      : dg::Entity (name),
	error_ (0,
		"TimeBasedGeneratorGain("
		+ name + ")::input(vector)::error"),
	gain_ (boost::bind
	       (&TimeBasedGeneratorGain::computeGain,
		this, _1, _2),
	       error_,
	       "TimeBasedGeneratorGain("
	       + name + ")::output(double)::gain")
    {
      signalRegistration (error_ << gain_);
    }

    TimeBasedGeneratorGain::~TimeBasedGeneratorGain ()
    {
    }

    double&
    TimeBasedGeneratorGain::computeGain (double& res, int)
    {
      res = 42.;
      return res;
    }

    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN
    (TimeBasedGeneratorGain,
     "TimeBasedGeneratorGain");
  } // end of namespace sot.
} // end of namespace dynamicgraph.
