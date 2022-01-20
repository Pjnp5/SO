BEGIN {
     FS = " ";
     f=1
     #pilot
     FieldSize[f++]=3;
     #hostess
     FieldSize[f++]=2;
     #passengers
     FieldSize[f++]=4;
     for(i=1;i<21;i++) {
        FieldSize[f++] = 3;
     }
     #stats
     FieldSize[f++]=4;
     for(i=1;i<3;i++) {
        FieldSize[f++] = 3;
     }
}

/.*/ {
    if(NF==26 && $1!="PT") {
#        print  "NOTFILTE " $0
        for(i=1; i<=26; i++) {
            if(i<24) {
               if($i==prev[i]) {
                 printf("%*s ",FieldSize[i],".")
               }
               else printf ("%*s ",FieldSize[i],$i) 
               prev[i] = $i
            }
            else {
               printf ("%*s ",FieldSize[i],$i) 
            }
        }
        printf("\n")
    }
    else print $0
}

END{
}
