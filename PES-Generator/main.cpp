#include <iostream>
#include <fstream>
#include <sstream>
#include <string>	
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <cmath>

#include "MyException.h"
#include "Expression.h"
#include "Token.h"
#include "Substitute.h"
#include "NewLiner.h"
#include "Generator.h"

using namespace std;


int dimP(0);
int dimR(0);
int dimX(0);
int maxExp(0);
int maxRab(0);

const string inDIR("input/");
const string outDIR("output/");
const string tmpDIR("tmp/");
const string templateDIR("template/");
const string indent = "    ";
string pes_name;

int main(int argc, char **argv)
{

	Expression::name = "p";
	Derivative::name = "dpdr";
	Primary::name = "r";

	try
	{
		if (argc < 2) {
			throw string("Error: missing oprand <pes name>!");
		}

		pes_name = argv[1];

		string inFile;
		string outFile;

		{
			ifstream fin;
			ofstream fout;

			inFile = inDIR + "FI.dat";
			fin.open(inFile.c_str(), ifstream::in);
			openCheck(fin, inFile);

			TokenStream tk(fin);
			vector<Expression> exprs;

			tk.get();
			while (tk.current().kind != Kind::end) {
				exprs.push_back(Expression());
				exprs.back().getExpr(tk);
			}
			fin.close();

			// ========== print FI.tmp ==========
			outFile = tmpDIR + "FI.out";
			fout.open(outFile.c_str(), ofstream::out);

			for (size_t i = 0; i < exprs.size(); ++i) {
				exprs[i].print(fout);
			}
			fout.close();

			// ========== print drdp.tmp ==========
			outFile = tmpDIR + "dpdr.out";
			fout.open(outFile.c_str(), ofstream::out);

			fout << Derivative::name << "=0.d0" << endl;
			for (size_t i = 0; i < exprs.size(); ++i) {
				for (int rid = 1; rid <= dimR; ++rid) {
					exprs[i].derivate(rid).print(fout);
				}
			}
			fout.close();

			string cmd;
			cmd = R"(sed -i 's/\*1\.d0//g' )" + outFile;
			system(cmd.c_str());
		}

		maxRab = dimR * (dimR - 1) / 2;

		//===========================================================
		//==================== Generate fi.f90 ====================
		{
			ifstream fin;
			ofstream fout;

			inFile = tmpDIR + "FI.out";
			outFile = tmpDIR + "FI.tmp";

			fin.open(inFile.c_str(), ifstream::in);
			openCheck(fin, inFile);
			fout.open(outFile.c_str(), ofstream::out);

			substitute(fin, fout);

			fin.close();
			fout.close();

			system((string("mv ") + outFile + ' ' + inFile).c_str());

			outFile = outDIR + "pes_calc_fi_" + pes_name + ".f90";

			fin.open(inFile.c_str(), ifstream::in);
			fout.open(outFile.c_str(), ofstream::out);

			fout << "subroutine pes_calc_fi_" << pes_name << '(';
			fout << "p,r,";
			for (int iExp = 2; iExp <= maxExp; ++iExp) {
				fout << 'r' << iExp << ',';
			}
			fout << "rab)" << endl;

			fout << "implicit none" << endl;
			fout << "real*8, intent(out) :: p(" << dimP << ')' << endl;
			fout << "real*8, intent(in)  :: r(" << dimR << ')' << endl;
			for (int iExp = 2; iExp <= maxExp; ++iExp) {
				fout << "real*8, intent(in)  :: r" << iExp << '(' << dimR << ')' << endl;
			}
			fout << "real*8, intent(in)  :: rab(" << maxRab << ')' << endl;
			fout << endl;

			newLiner(fin, fout);

			fout << endl;
			fout << "end subroutine pes_calc_fi_" << pes_name << endl;

			fin.close();
			fout.close();

			system((string("rm ") + inFile).c_str());
		}

		//===========================================================
		//==================== Generate dpdr.f90 ====================
		{
			ifstream fin;
			ofstream fout;

			inFile = tmpDIR + "dpdr.out";
			outFile = tmpDIR + "dpdr.tmp";

			fin.open(inFile.c_str(), ifstream::in);
			openCheck(fin, inFile);
			fout.open(outFile.c_str(), ofstream::out);

			substitute(fin, fout);

			fin.close();
			fout.close();

			system((string("mv ") + outFile + ' ' + inFile).c_str());

			outFile = outDIR + "pes_calc_dpdr_" + pes_name + ".f90";
			fin.open(inFile.c_str(), ifstream::in);
			fout.open(outFile.c_str(), ofstream::out);

			fout << "subroutine pes_calc_dpdr_" << pes_name << '(';
			fout << "dpdr,r,";
			for (int iExp = 2; iExp <= maxExp - 1; ++iExp) {
				fout << 'r' << iExp << ',';
			}
			fout << "rab)" << endl;
			fout << "implicit none" << endl;
			fout << "real*8, intent(out) :: dpdr(" << dimP << ',' << dimR << ')' << endl;
			fout << "real*8, intent(in)  :: r(" << dimR << ')' << endl;
			for (int iExp = 2; iExp <= maxExp - 1; ++iExp) {
				fout << "real*8, intent(in)  :: r" << iExp << '(' << dimR << ')' << endl;
			}
			fout << "real*8, intent(in)  :: rab(" << maxRab << ')' << endl;
			fout << endl;

			newLiner(fin, fout);

			fout << endl;
			fout << "end subroutine pes_calc_dpdr_" << pes_name << endl;

			fin.close();
			fout.close();

			system((string("rm ") + inFile).c_str());
		}

		dimX = static_cast<int>(sqrt(dimR * 2)) + 1;

		//===========================================================
		//==================== Generate drdx.f90 ====================
		{
			outFile = outDIR + "pes_calc_drdx_" + pes_name + ".f90";
			ofstream fout(outFile.c_str(), ofstream::out);

			fout << "subroutine pes_calc_drdx_" << pes_name << "(drdx,x,r,r3)" << endl;
			fout << indent << "implicit none" << endl;
			fout << indent << "real*8, intent(out) :: drdx(3," << dimX << ',' << dimR << ')' << endl;
			fout << indent << "real*8, intent(in)  :: x(3," << dimX << ')' << endl;
			fout << indent << "real*8, intent(in)  :: r(" << dimR << ')' << endl;
			fout << indent << "real*8, intent(in)  :: r3(" << dimR << ')' << endl;
			fout << endl;

			fout << indent << "drdx = 0.d0" << endl;

			int iR = 0;
			for (int iX = 1; iX <= dimX; ++iX) {
				for (int jX = iX + 1; jX <= dimX; ++jX) {
					iR++;
					for (int i = 1; i <= 3; ++i) {
						fout << indent;
						fout << "drdx(" << i << ',' << iX << ',' << iR << ") = ";
						fout << "r3(" << iR << ") * ";
						fout << "(x(" << i << ',' << jX << ") - x(" << i << ',' << iX << "))";
						fout << endl;

						fout << indent;
						fout << "drdx(" << i << ',' << jX << ',' << iR << ") = ";
						fout << "r3(" << iR << ") * ";
						fout << "(x(" << i << ',' << iX << ") - x(" << i << ',' << jX << "))";
						fout << endl;
					}
				}
			}

			fout << endl;
			fout << "end subroutine pes_calc_drdx_" << pes_name << endl;

			fout.close();
		}

		//==========================================================
		//==================== read network.dat ====================
		readPesParameter();

		//==========================================================
		//==================== Generate pes.mod ====================
		generatePesMod();

		return 0;
	}
	catch (MyException & e)
	{
		cerr << e.what() << endl;
		return 127;
	}
}

