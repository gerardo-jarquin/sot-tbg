#ifndef SOT_TBG_TIME_BASED_GENERATOR_HH
# define SOT_TBG_TIME_BASED_GENERATOR_HH
# include <jrl/mal/boost.hh>
# include <dynamic-graph/entity.h>
# include <dynamic-graph/signal-ptr.h>
# include <dynamic-graph/signal-time-dependent.h>

namespace dynamicgraph
{
  namespace sot
  {
    namespace ml = maal::boost;
    namespace dg = dynamicgraph;

    class TimeBasedGeneratorGain : public dg::Entity
    {
      DYNAMIC_GRAPH_ENTITY_DECL();

    public:
      explicit TimeBasedGeneratorGain (const std::string& name);
      ~TimeBasedGeneratorGain ();

    private:
      double& computeGain (double& res, int t);

      dg::SignalPtr<ml::Vector, int> error_;
      dg::SignalTimeDependent<double, int> gain_;
    };

  } // end of namespace sot.
} // end of namespace dynamicgraph.

#endif //! SOT_TBG_TIME_BASED_GENERATOR_HH
