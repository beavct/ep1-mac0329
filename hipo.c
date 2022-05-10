#include <stdio.h>
    #include <stdlib.h>
    
    #include <ctype.h>
    
    /* Código de instrução */
    #define CI_CEA 11
    #define CI_CAE 12
    #define CI_SOM 21
    #define CI_SUB 22
    #define CI_MUL 23
    #define CI_DIV 24
    #define CI_MOD 25
    #define CI_LER 31
    #define CI_IMP 41
    #define CI_NOP 50
    #define CI_DES 51
    #define CI_DPO 52
    #define CI_DPZ 53
    #define CI_DNE 54
    #define CI_DNZ 55
    #define CI_DDZ 56
    #define CI_DZZ 57
    #define CI_DDF 58
    #define CI_DFF 59
    #define CI_ADE 61
    #define CI_ADD 62
    #define CI_PAR 70
    
    typedef struct digito {
        int sinal, valor;
    } tdigito;
    
    typedef struct longdigito {
        int sinal;
        long int valor;
    } tldigito;
    
    void LimpaTela ()
    {
        printf("\n\n\n\n\n\n\n\n\n\n\n");
    }
    
    void EscreveDigito (struct digito dado)
    {
        if (dado.sinal==1) putchar ('+');
        else putchar ('-');
        printf ("%04d", dado.valor);
    }
    
    void EscreveLongDigito (struct longdigito dado)
    {
        if (dado.sinal==1) putchar ('+');
        else putchar ('-');
        printf ("%08ld", dado.valor);
    }
    
    void MostraInstrucoes (void)
    {
        LimpaTela();
        printf ("SPC para executar instrucao, G para executar ate o fim, \
    Q para terminar.");
    }
    
    void MostraEstado (struct digito *memoria, int AI, struct longdigito acumulador)
    {
        int i, j, EE;
        char delimit_esq[5];
        char delimit_dir[5];
    
        delimit_esq[1] = delimit_dir[1] = '\0';
                    
        printf ("\n\nacumulador=");
        EscreveLongDigito (acumulador);
        printf ("  AI=%+05d.\n\n  ", AI);
        for (j=0;j<10;j++) {
            printf ("      %1d", j);
        }
        EE=memoria[AI].valor%100;
        for (i=0;i<10;i++) {
            printf ("\n%2d= ", 10*i);
            for (j=0;j<10;j++) {
                if ((i*10+j)==EE){
                    delimit_esq[0] = '('; 
                    delimit_dir[0] = ')'; 
                }
                else if ((i*10+j)==AI) {
                    delimit_esq[0] = '<'; 
                    delimit_dir[0] = '>'; 
                }
                else {
                    delimit_esq[0] = ' '; 
                    delimit_dir[0] = ' '; 
                }
                printf("%s", delimit_esq);
                EscreveDigito(memoria[i*10+j]);
                printf("%s", delimit_dir);
            }
        }
        printf("\n\n\n\n\n\n\n\n\n");
    }
    
    void Ler (struct digito *dado)
    {
        int v;
        printf ("Digite o numero. ");
        scanf ("%d", &v);
        if (v!=0) {              
            dado->valor=abs (v);
            dado->sinal=v/dado->valor;
        }
        else{
            dado->valor=v;
            dado->sinal=1;
        }
    }
    
    void Imprime (struct digito dado)
    {
        EscreveDigito (dado);
        printf ("\nAperte ENTER para continuar");
        getchar();
    }
    
    void Carrega (struct digito *memoria)
    {
        FILE *fp;
        char NomeDoArquivo[128], c;
        int EE, dado;
        printf ("Digite o nome do arquivo de programa. ");
        scanf ("%s", NomeDoArquivo);
        fp=fopen (NomeDoArquivo, "rt");
        if (!fp) {
            perror (NomeDoArquivo);
            exit (0);
        }
        for (EE=0;EE<100;EE++) {
            memoria[EE].valor=0;
            memoria[EE].sinal=1;
        }
        while (fscanf (fp, "%d %c%d", &EE, &c, &dado)==3) {
            if (c=='+') memoria[EE].sinal=1;
            else memoria[EE].sinal=-1;
            memoria[EE].valor=dado%10000;
        }
        fclose (fp);
    }
    
    int main(void)
    {
        int II, EE, AI;
        struct digito memoria[100];
        struct longdigito acumulador;
        int ExecutaAteOFim;
        char c;
        long int v;
        /* Reset */
        AI = 0;
        acumulador.sinal = 0;
        acumulador.valor = 0;
        II = 0;
        EE = 0;
        Carrega (memoria);
        ExecutaAteOFim=0;
        /* O estouro do apontador de instrucoes nao eh
           condicaoo de parada para um programa nem
           excecao (erro) na CPU, mas aqui vamos supor
           que seja. */
        do {
            MostraInstrucoes ();
            MostraEstado (memoria, AI, acumulador);
            /* Controle do simulador */
            if (!ExecutaAteOFim) {
                puts ("Tecle a opcao e ENTER para continuar ou ENTER para o proximo passo.");
                c=getchar();
                c=toupper (c);
                switch (c) {
                    case 'G': {
                        ExecutaAteOFim=1;
                        break;
                    }
                    case 'Q': {
                        exit (0);
                        break;
                    }
                    default: break;
                }
            }
            /* Decodifica Instrução */
            II = memoria[AI].valor / 100;
            EE = memoria[AI].valor % 100;
            /* Incrementa contador de instruções */
            AI++;
            /* Executa instrução */
            switch (II) {
                case CI_CEA: {
                    acumulador.valor = memoria[EE].valor;
                    acumulador.sinal = memoria[EE].sinal;
                    break;
                }
                case CI_CAE: {
                    memoria[EE].valor = acumulador.valor%10000;
                    memoria[EE].sinal = acumulador.sinal;
                    break;
                }
                case CI_SOM: {
                    if ((acumulador.sinal!=-1) || (acumulador.valor!=0)) {
                        v = acumulador.sinal*acumulador.valor +
                        memoria[EE].sinal*memoria[EE].valor;
                        if (v/100000000l) {
                            acumulador.sinal=-1;
                            acumulador.valor=0;
                        }
                        else {
                            if (v>=0) acumulador.sinal = 1;
                            else acumulador.sinal = -1;
                            acumulador.valor = labs(v);
                        }
                    }
                    break;
                }
                case CI_SUB: {
                    if ((acumulador.sinal!=-1) || (acumulador.valor!=0)) {
                        v = acumulador.sinal*acumulador.valor -
                            memoria[EE].sinal*memoria[EE].valor;
                        if (v/100000000l) {
                            acumulador.sinal=-1;
                            acumulador.valor=0;
                        }
                        else {
                            if (v>=0) acumulador.sinal = 1;
                            else acumulador.sinal = -1;
                            acumulador.valor = labs(v);
                        }
                    }
                    break;
                }
                case CI_MUL: {
                    if ((acumulador.sinal!=-1) || (acumulador.valor!=0)) {
                        v = (acumulador.sinal*acumulador.valor) *
                            (memoria[EE].sinal*memoria[EE].valor);
                        if (v/100000000l) {
                            acumulador.sinal=-1;
                            acumulador.valor=0;
                        }
                        else {
                            if (v>=0) acumulador.sinal = 1;
                            else acumulador.sinal = -1;
                            acumulador.valor = labs(v);
                        }
                    }
                    break;
                }
                case CI_DIV: {
                    if ((acumulador.sinal!=-1) || (acumulador.valor!=0)) {
                        v = (acumulador.sinal*acumulador.valor) /
                            (memoria[EE].sinal*memoria[EE].valor);
                        if (v/100000000l) {
                            acumulador.sinal=-1;
                            acumulador.valor=0;
                        }
                        else {
                            if (v>=0) acumulador.sinal = 1;
                            else acumulador.sinal = -1;
                            acumulador.valor = labs(v);
                        }
                    }
                    break;
                }
                case CI_MOD: {
                    if ((acumulador.sinal!=-1) || (acumulador.valor!=0)) {
                        v = (acumulador.valor*acumulador.sinal) %
                        (memoria[EE].sinal*memoria[EE].valor);
                        if (v>=0) acumulador.sinal = 1;
                        else acumulador.sinal = -1;
                        acumulador.valor=labs(v);
                    }
                    break;
                }
                case CI_LER: {
                    Ler (&(memoria[EE]));
                    break;
                }
                case CI_IMP: {
                    Imprime (memoria[EE]);
                    break;
                }
                case CI_NOP: break;
                case CI_DES: {
                    AI = EE;
                    break;
                }
                case CI_DPO: {
                    if ((acumulador.sinal == 1) && (acumulador.valor!=0)) AI = EE;
                    break;
                }
                case CI_DPZ: {
                    if (acumulador.sinal == 1) AI = EE;
                    break;
                }
                case CI_DNE: {
                    if ((acumulador.sinal == -1) && (acumulador.valor !=0)) AI = EE;
                    break;
                }
                case CI_DNZ: {
                    if (((acumulador.sinal == -1) && (acumulador.valor !=0)) ||
                        ((acumulador.sinal ==  1) && (acumulador.valor ==0))) AI = EE;
                    break;
                }
                case CI_DDZ: {
                    if ((acumulador.sinal!=1) || (acumulador.valor != 0)) AI = EE;
                    break;
                }
                case CI_DZZ: {
                    if ((acumulador.sinal==1) && (acumulador.valor == 0)) AI = EE;
                    break;
                }
                case CI_DDF: {
                    if ((acumulador.sinal!=-1) ||(acumulador.valor!=0)) AI = EE;
                    break;
                }
                case CI_DFF: {
                    if ((acumulador.sinal==-1) &&(acumulador.valor==0)) AI = EE;
                    break;
                }
                case CI_ADE: {
                    acumulador.valor = (acumulador.valor * 10) % 100000000;
                    break;
                }
                case CI_ADD: {
                    acumulador.valor = acumulador.valor / 10;
                    break;
                }
            }
        } while ((AI < 100) && (II != CI_PAR));
        return 0;
    }
    