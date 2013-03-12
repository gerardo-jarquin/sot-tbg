#include <boost/bind.hpp>
#include <dynamic-graph/factory.h>


#include <dynamic-graph/command.h>
#include <dynamic-graph/command-setter.h>
#include <dynamic-graph/command-getter.h>
#include <dynamic-graph/command-bind.h>
#include <dynamic-graph/all-commands.h>

#include "time-based-generator.hh"

namespace dynamicgraph
{
    namespace sot
    {    
        void TimeBasedGeneratorGain::addCommands()
        {
            using namespace ::dynamicgraph::command;
            std::string docstring;
            // Command SetParameters
            docstring = "    \n"
              "    set configuration parameters\n"
              "      Input:\n"
              "        floating point value: desired final time"
              "        floating point value: time period for which the task should remain completed"
              "        floating point value: time of activation of the TBG gain"
              "        floating point value: final error tolerance"
              "    \n";
            addCommand("setTBGparameters", 
	                    makeCommandVoid4(*this, 
	                                    &TimeBasedGeneratorGain::setParameters, 
	                                    docstring));
            
            // Command SetActivationTime
            docstring = "    \n"
              "    set the activation time of the TBG gain\n"
              "      Input:\n"
              "        floating point value: activation time of the TBG gain"
              "    \n";
            addCommand("setInitialTime", 
                        makeCommandVoid1(*this, &TimeBasedGeneratorGain::setActivationTime, docstring));
            
            // Command SetFinalTime
            docstring = "    \n"
              "    set the desired final time\n"
              "      Input:\n"
              "        floating point value: desired final time"
              "    \n";
            addCommand("setFinalTime", 
                        makeCommandVoid1(*this, &TimeBasedGeneratorGain::setFinalTime, docstring));
            
            // Command SetActiveTimePeriod
            docstring = "    \n"
              "    set the time period in which the task should remain active after the final time\n"
              "      Input:\n"
              "        floating point value: time period for which the task should remain completed"
              "    \n";
            addCommand("setActivePeriod", 
                        makeCommandVoid1(*this, &TimeBasedGeneratorGain::setActiveTimePeriod, docstring));
            
            // Command SetTolerance
            docstring = "    \n"
              "    set the final error tolerance, the value should be positive\n"
              "      Input:\n"
              "        floating point value: final error tolerance"
              "    \n";
            addCommand("setErrorTolerance", 
                        makeCommandVoid1(*this, &TimeBasedGeneratorGain::setTolerance, docstring));
                        
            // Command display
            docstring = "    \n"
              "    print the parameters of the TBG gain\n"
              "    \n";
            addCommand("display", 
                        makeCommandVoid0(*this, &TimeBasedGeneratorGain::display, docstring));
        }
        
        TimeBasedGeneratorGain::TimeBasedGeneratorGain (const std::string& name) 
        : dg::Entity (name),
	    error_ (0, "TimeBasedGeneratorGain(" + name + ")::input(vector)::error"),
	    gain_ (boost::bind(&TimeBasedGeneratorGain::computeGain,this, _1, _2),
	           error_,
	           "TimeBasedGeneratorGain(" + name + ")::output(double)::gain")
        {
            setParameters( 2, 0, 0, 0.0001);
            signalRegistration (error_ << gain_);
            addCommands();
        }
    
        TimeBasedGeneratorGain::TimeBasedGeneratorGain (const std::string& name, 
                                                        const float& t_b_) 
        : dg::Entity (name),
	    error_ (0, "TimeBasedGeneratorGain(" + name + ")::input(vector)::error"),
	    gain_ (boost::bind(&TimeBasedGeneratorGain::computeGain,this, _1, _2),
	           error_,
	           "TimeBasedGeneratorGain(" + name + ")::output(double)::gain")
        {
            setParameters( t_b_, 0, t_b_-2, 0.0001);
            signalRegistration (error_ << gain_);
            addCommands();
        }
        
        TimeBasedGeneratorGain::TimeBasedGeneratorGain (const std::string& name, 
                                                        const float& t_b_,
                                                        const float& t_c_) 
        : dg::Entity (name),
	    error_ (0, "TimeBasedGeneratorGain(" + name + ")::input(vector)::error"),
	    gain_ (boost::bind(&TimeBasedGeneratorGain::computeGain,this, _1, _2),
	           error_,
	           "TimeBasedGeneratorGain(" + name + ")::output(double)::gain")
        {
            setParameters( t_b_, t_c_, t_b_-2, 0.0001);
            signalRegistration (error_ << gain_);
            addCommands();
        }
        
        TimeBasedGeneratorGain::TimeBasedGeneratorGain (const std::string& name, 
                                                        const float& t_b_,
                                                        const float& t_c_,
                                                        const float& t_0_) 
        : dg::Entity (name),
	    error_ (0, "TimeBasedGeneratorGain(" + name + ")::input(vector)::error"),
	    gain_ (boost::bind(&TimeBasedGeneratorGain::computeGain,this, _1, _2),
	           error_,
	           "TimeBasedGeneratorGain(" + name + ")::output(double)::gain")
        {
            setParameters( t_b_, t_c_, t_0_, 0.0001);
            signalRegistration (error_ << gain_);
            addCommands();
        }

        TimeBasedGeneratorGain::~TimeBasedGeneratorGain ()
        {
        }

        double& TimeBasedGeneratorGain::computeGain (double& res, int t)
        {
            double time = t * 0.005;
            if ( time < t_0 )
                res = 0;
            else if ( (time <= t_b) && (time >= t_0) ) 
                computeAlpha_t (res, time);
            else {
                if ( t_c > 0 )
                    computeActiveGain(res, time);
                else 
                    res = 0;
            }
            return res;
        }  
        
        
        void TimeBasedGeneratorGain::computeAlpha_t (double& alpha_t, 
                                                     const double& t)
        {
            double Xi;
            double t_1 = t - t_0;
            double t_2 = t_b - t_0;
            double Xi_dot =   30*( ( pow(t_1, 2) ) / ( pow(t_2, 3) ) ) 
                            - 60*( ( pow(t_1, 3) ) / ( pow(t_2, 4) ) )
                            + 30*( ( pow(t_1, 4) ) / ( pow(t_2, 5) ) );
            computeXi( Xi, t_0, t_b, t ); 
            alpha_t = alpha_0*( Xi_dot/(1 - Xi + delta) );
        }
        
        void TimeBasedGeneratorGain::computeActiveGain(double& activeGain, 
                                                       const double& t)
        {
            double t_r = t_b + t_c;
            double t_1 = t_b + 0.5;
            double t_2 = t_r - 0.5;
            if (t >= t_1 && t <= t_2)
                activeGain = 10;
            else if ( t >= t_r )
                activeGain = 0;
            else
            {
                if ( t < t_1 )
                    computeXi( activeGain, t_b, t_1, t );
                else
                {
                    double auxGain;
                    computeXi( auxGain, t_2, t_r, t );
                    activeGain = 1-auxGain;
                }
                activeGain = (activeGain)*10
            }
        }
           
        void TimeBasedGeneratorGain::computeXi(double& valueXi, 
                                               const double& t_ini,
                                               const double& t_fin,
                                               const double& t)
        {
        
            double t_1 = t - t_ini;
            double t_2 = t_fin - t_ini;
            valueXi       =   10*( ( pow(t_1, 3) ) / ( pow(t_2, 3) ) ) 
                            - 15*( ( pow(t_1, 4) ) / ( pow(t_2, 4) ) )
                            +  6*( ( pow(t_1, 5) ) / ( pow(t_2, 5) ) );
        }
    
        void TimeBasedGeneratorGain::setParameters( const float& t_b_,
	                                                const float& t_c_,
	                                                const float& t_0_,
	                                                const double& delta_)
        {
            if ( t_0_ >=0 )  t_0 = t_0_;
            else t_0 = 0;
            setFinalTime (t_b_);
	        setTolerance ( delta_ );
            setActiveTimePeriod ( t_c_ );
            alpha_0 = 1.00001;
        }
        
        void TimeBasedGeneratorGain::display( void )
        {
            std::cout<<"TBG gain parameters: "<<std::endl;
            std::cout<<"    desired final time = "<<t_b<<std::endl;
            std::cout<<"    TBG gain activation time = "<<t_0<<std::endl;
            std::cout<<"    Active time period after final time = "<<t_c<<std::endl;
            std::cout<<"    error tolerance = "<<delta<<std::endl;
        }

        DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(TimeBasedGeneratorGain, 
                                           "TimeBasedGeneratorGain");
    } // end of namespace sot.
} // end of namespace dynamicgraph.
