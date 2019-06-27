#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{

	if (argc < 2) {
		cerr << "Error: missing oprand!" << endl;
		return 127;
	}

	int nX;
	int nR;
	const string indent = "    ";

	istringstream sin;
	sin.str(argv[1]);

	sin >> nX;
	nR = nX * (nX - 1) / 2;

	ofstream fout("drdx.f90", ofstream::out);

	fout << "subroutine pes_calc_drdx_xxxxx(r,x,drdx)" << endl;
	fout << indent << "implicit none" << endl;
	fout << indent << "real*8, intent(in)  :: r(" << nR << ')' << endl;
	fout << indent << "real*8, intent(in)  :: x(3," << nX << ')' << endl;
	fout << indent << "real*8, intent(out) :: drdx(3," << nX << ',' << nR << ')' << endl;
	fout << indent << "real*8 :: r3(" << nR << ')' << endl;
	fout << endl;

	fout << indent << "r3   = r**3" << endl;
	fout << indent << "drdx = 0.d0" << endl;

	int iR = 0;
	for (int iX = 1; iX <= nX; ++iX) {
		for (int jX = iX + 1; jX <= nX; ++jX) {
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
	fout << "end subroutine pes_calc_drdx_xxxxx" << endl;

	fout.close();
	return 0;
}
