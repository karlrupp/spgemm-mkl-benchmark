
mkdir -p matrices/
cd matrices/

## more regular matrices:
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/cant.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/mac_econ_fwd500.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/mc2depi.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Bova/rma10.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/pdb1HYS.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/QCD/conf5_4-8x8-05.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/DNVS/shipsec1.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/consph.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Boeing/pwtk.tar.gz

## more irregular matrices:
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/cop20k_A.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/amazon0312.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/ca-CondMat.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/cit-Patents.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Hamm/scircuit.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/email-Enron.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/p2p-Gnutella31.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/roadNet-CA.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/Williams/webbase-1M.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/web-Google.tar.gz
wget -nc http://www.cise.ufl.edu/research/sparse/MM/SNAP/wiki-Vote.tar.gz


## Extract:
for f in `ls *.gz`
do
  tar -xzf $f --strip-components 1   # strips subdirectory structure
done



