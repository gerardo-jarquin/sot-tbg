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

    /* --- CONSTRUCTORS ADN DESTRUCTOR---- */
    public:
      explicit TimeBasedGeneratorGain (const std::string& name);
      explicit TimeBasedGeneratorGain (const std::string& name, 
                                       const float& t_b_);
      explicit TimeBasedGeneratorGain (const std::string& name, 
                                       const float& t_b_,
	                                   const float& t_c_);
      explicit TimeBasedGeneratorGain (const std::string& name, 
                                       const float& t_b_,
	                                   const float& t_c_,
	                                   const float& t_0_);
      ~TimeBasedGeneratorGain ();

    /* --- METHODS---- */
    public:
      void setParameters( const float& t_b_,
	                      const float& t_c_,
	                      const float& t_0_,
	                      const double& delta_);
	                      
	  inline void setActivationTime (const float& t_0_)
      {
        if ( t_0_ >=0 && t_0_ <= t_b - 0.5)  t_0 = t_0_;
        else {
            if ( t_b - 2 > 0 ) t_0 = t_b - 2;  //security value
            else {
                t_0 = 0;  //security value
                t_b = 2;  //security value
            }
        }
      }                    
	                               
      inline void setFinalTime (const float& t_b_)
      {
        if ( t_b_ >= t_0 + 0.5 )  t_b = t_b_;
        else t_b = t_0 + 2;    //security value
      }
      
      inline void setTolerance (const double& delta_)
      {
        if ( delta_ > 0 && delta_ <= 0.1 )  delta = delta_;
        else delta = 0.0001;
      }
      
      inline void setActiveTimePeriod (const float& t_c_)
      {
        if ( t_c_ >= 0)  t_c = t_c_;
        else t_c = 0;
      }
      
      void display( void );
      
    private:
      double& computeGain (double& res, int t);
      void computeAlpha_t (double& alpha_t, const double& t);
      void computeActiveGain(double& activeGain, const double& t);
      void computeXi(double& valueXi, 
                     const double& t_ini,
                     const double& t_fin,
                     const double& t);
      void addCommands();

    /* --- SIGNALS --- */
    public:
      dg::SignalPtr<ml::Vector, int> error_;
      dg::SignalTimeDependent<double, int> gain_;
      
    /* --- TBG Parameters --- */
    float t_b;       //desired convergence time
    float t_c;       //period of time that the task should remain completed
    double alpha_0;  //main TBG gain, 
    float t_0;       //TBG gain activation time
    double delta;    //Error tolerance, this value must be greater than 0
      
    };

  } // end of namespace sot.
} // end of namespace dynamicgraph.

#endif //! SOT_TBG_TIME_BASED_GENERATOR_HH
