#! /bin/bash
# andrew@rocketnumbernine.com - Use freely, for whatever, at your own risk.

EAGLE="/Applications/EAGLE/EAGLE.app/Contents/MacOS/EAGLE"

if [ $# -lt 1 ] ; then
        echo "usage $0 <path-to-board> [output-dir-and-file-prefix]"
        echo " e.g:  $0 my_boards/boardx.brd gerbers/board"
        echo "  will create gerber files with eagle in a directory called gerbers"
        echo "  and bundle into gerbers.zip"
        exit 1
fi

board="$1"
outputfile=${2:-"$(dirname $board)/$(basename $board .brd)"}
outputdir=$(dirname ${outputfile})

if [ ! -d ${outputdir} ]; then
        mkdir -p ${outputdir}
fi
set -e

# create top copper (cmp), bottom copper (sol), top solder mask (stc), bottom solder mask (sts), {top} silkscreen (plc)
${EAGLE} -X -dGERBER_RS274X -o${outputfile}.cmp ${board} Top Pads Vias
${EAGLE} -X -dGERBER_RS274X -o${outputfile}.sol ${board} Bottom Pads Vias
${EAGLE} -X -dGERBER_RS274X -o${outputfile}.stc ${board} tStop
${EAGLE} -X -dGERBER_RS274X -o${outputfile}.sts ${board} bStop
${EAGLE} -X -dGERBER_RS274X -o${outputfile}.plc ${board} Dimension tPlace tNames

# create drill files
${EAGLE} -X -dEXCELLON -o${outputfile}.drd ${board} Drills Holes
# get drills used from dri file and create drl rack file 
# (like running drillcfg.ulp: ${EAGLE} -N- -C'RUN drillcfg.ulp; QUIT;' -o${outputfile} ${board}
cat ${outputfile}.dri | sed -e 's/ *\(T[0-9][0-9]\) *\([0-9.]*..\).*/\1 \2/' | grep -e "^T[0-9][0-9]" > ${outputfile}.drl

zip -r ${outputdir}.zip ${outputdir}

