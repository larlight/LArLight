
export USER_MODULE="FANN ClusterRecoUtil CMTool AnalysisAlg ShowerReco3D McshowerLookback CMToolAna"
# Find the location of this script:
me="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# Find the directory one above.
export MAKE_TOP_DIR="$( cd "$( dirname "$me" )" && pwd )"

source $MAKE_TOP_DIR/config/setup.sh
