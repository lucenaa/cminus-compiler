./killbuild -> na raiz

cd build

cmake ..

make ddiff

cd ..

python compare_diff.py


vai ter criado os outputs gerados pelo código e os diffs

-> os seus outputs estarão em alunoout

-> o gabarito está em output

-> os scripts de test estão em example

