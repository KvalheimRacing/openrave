// Copyright (C) 2006-2008 Carnegie Mellon University (rdiankov@cs.cmu.edu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "plugindefs.h"
#include "ikbase.h"

#include "ikfastproblem.h"

namespace barrettwam {
#include "ik_barrettwam.h"
}
namespace pa10 {
#include "ik_pa10.h"
}
namespace puma {
#include "ik_puma.h"
}
namespace manusleft {
#include "ik_manusleftarm.h"
}
namespace man1left {
#include "ik_man1left.h"
}
namespace man1right {
#include "ik_man1right.h"
}
namespace katana {
#include "ik_katana.h"
}

// register for typeof (MSVC only)
#ifdef RAVE_REGISTER_BOOST
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
BOOST_TYPEOF_REGISTER_TYPE(barrettwam::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(pa10::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(puma::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(manusleft::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(man1left::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(man1right::IKSolution)
BOOST_TYPEOF_REGISTER_TYPE(katana::IKSolution)
#endif

#include "ikfastproblem.h"

RAVE_PLUGIN_API InterfaceBasePtr CreateInterface(InterfaceType type, const std::string& name, const char* pluginhash, EnvironmentBasePtr penv)
{
    if( strcmp(pluginhash,RaveGetInterfaceHash(type)) ) {
        RAVELOG_WARNA("plugin type hash is wrong\n");
        throw openrave_exception("bad plugin hash");
    }
    if( !penv )
        return InterfaceBasePtr();
    
    stringstream ss(name);
    string interfacename;
    ss >> interfacename;
    std::transform(interfacename.begin(), interfacename.end(), interfacename.begin(), ::tolower);

    switch(type) {
    case PT_InverseKinematicsSolver: {
        dReal freeinc = 0.04f;
        wstring name_ik;
        ss >> freeinc;
        
        if( interfacename == "wam7ikfast" ) {
            vector<int> vfree(barrettwam::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = barrettwam::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<barrettwam::IKReal,barrettwam::IKSolution>(barrettwam::ik,vfree,freeinc,barrettwam::getNumJoints(),(IkParameterization::Type)barrettwam::getIKType(), penv));
        }
        else if( interfacename == "pa10ikfast" ) {
            vector<int> vfree(pa10::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = pa10::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<pa10::IKReal,pa10::IKSolution>(pa10::ik,vfree,freeinc,pa10::getNumJoints(),(IkParameterization::Type)pa10::getIKType(), penv));
        }
        else if( interfacename == "pumaikfast" ) {
            vector<int> vfree(puma::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = puma::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<puma::IKReal,puma::IKSolution>(puma::ik,vfree,freeinc,puma::getNumJoints(),(IkParameterization::Type)puma::getIKType(), penv));
        }
        else if( interfacename == "manusleftarmikfast" ) {
            vector<int> vfree(manusleft::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = manusleft::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<manusleft::IKReal,manusleft::IKSolution>(manusleft::ik,vfree,freeinc,manusleft::getNumJoints(),(IkParameterization::Type)manusleft::getIKType(), penv));
        }
        else if( interfacename == "man1leftikfast" ) {
            vector<int> vfree(man1left::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = man1left::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<man1left::IKReal,man1left::IKSolution>(man1left::ik,vfree,freeinc,man1left::getNumJoints(),(IkParameterization::Type)man1left::getIKType(), penv));
        }
        else if( interfacename == "man1rightikfast" ) {
            vector<int> vfree(man1right::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = man1right::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<man1right::IKReal,man1right::IKSolution>(man1right::ik,vfree,freeinc,man1right::getNumJoints(),(IkParameterization::Type)man1right::getIKType(), penv));
        }
        else if( interfacename == "katanaikfast" ) {
            vector<int> vfree(katana::getNumFreeParameters());
            for(size_t i = 0; i < vfree.size(); ++i)
                vfree[i] = katana::getFreeParameters()[i];
            return InterfaceBasePtr(new IkFastSolver<katana::IKReal,katana::IKSolution>(katana::ik,vfree,freeinc,katana::getNumJoints(),(IkParameterization::Type)katana::getIKType(), penv));
        }

        // look at all the ikfast problem solvers
        FOREACHC(itprob, IKFastProblem::GetProblems()) {
            IkSolverBasePtr psolver = (*itprob)->CreateIkSolver(interfacename, freeinc, penv);
            if( !!psolver )
                return psolver;
        }
        
        break;
    }        
    case PT_ProblemInstance:
        if( interfacename == "ikfast")
            return InterfaceBasePtr(new IKFastProblem(penv));
        break;
    default:
        break;
    }

    return InterfaceBasePtr();
}

RAVE_PLUGIN_API bool GetPluginAttributes(PLUGININFO* pinfo, int size)
{
    if( pinfo == NULL ) return false;
    if( size != sizeof(PLUGININFO) ) {
        RAVELOG_ERRORA("bad plugin info sizes %d != %d\n", size, sizeof(PLUGININFO));
        return false;
    }

    // fill pinfo
    pinfo->interfacenames[PT_ProblemInstance].push_back("IKFast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("WAM7ikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("PA10ikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("PUMAikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("ManusLeftArmikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("Man1Leftikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("Man1Rightikfast");
    pinfo->interfacenames[PT_InverseKinematicsSolver].push_back("Katanaikfast");
    return true;
}

RAVE_PLUGIN_API void DestroyPlugin()
{
}
