//---------------------------------------------------------------------------

#ifndef RatTanksOperatorSignedInH
#define RatTanksOperatorSignedInH
//---------------------------------------------------------------------------

#include <map>
#include <string>


namespace RationaliseTanks
{

class operatorSignedIn
{
public:
	operatorSignedIn();
	~operatorSignedIn();
    //the operator signed on
	std::string getOperator_cid() {return m_operator_cid;}
	//the machine the operator is on
	std::string getMachine_cid() {return m_machine_cid;}

	void setOperator_cid(const std::string &newOp)
	{
		m_operator_cid = newOp;
		m_hasSignedIn = false;
	}

	void setMachine_cid(const std::string &newMachine)
	{
		m_machine_cid = newMachine;
		m_hasSignedIn = false;
	}

	void setPremissions(const std::map<std::string, std::map<std::string,std::string> > &prems)
	{
		m_operatorPremissions = prems;
	}

	bool hasSignedIn() {return m_hasSignedIn;}

	std::map<std::string, std::map<std::string,std::string> >::iterator begin() {return m_operatorPremissions.begin();}
	std::map<std::string, std::map<std::string,std::string> >::iterator end() {return m_operatorPremissions.end();}

	bool hasPermissionToSignIn();
	bool hasPermissionToRationalise(const std::string &targetProject_cid);

private:
	bool m_hasSignedIn;
	std::string m_operator_cid;
	std::string m_machine_cid;
	std::map<std::string, std::map<std::string,std::string> > m_operatorPremissions;
};
}
//---------------------------------------------------------------------------

#endif
