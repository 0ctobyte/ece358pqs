EXT=dot
PNG_FILES=""
for i in *.${EXT}; do
	dot -Tpng $i -o ${i%.${EXT}}.png
	PNG_FILES=$PNG_FILES" ${i%.${EXT}}.png"
done

convert $PNG_FILES pq.pdf

open pq.pdf

rm *.png *dot

