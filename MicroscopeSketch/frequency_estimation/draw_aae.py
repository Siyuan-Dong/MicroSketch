#!/usr/bin/env python
# coding=utf-8

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import argparse

def main():
    parser = argparse.ArgumentParser(description="draw results of a single flow")
    parser.add_argument('--k', default="imc", type=str, help='print info of the k_th flow')
    parser.add_argument('--log', default=True, type=bool, help='log x-axis')
    global args
    args = parser.parse_args()


    # est_file_5000 = u'./result/top-' + args.k + u'_est_5000.csv'
    # est_data_5000 = pd.read_csv(est_file_5000)
    # est_file_7000 = u'./result/top-' + args.k + u'_est_7000.csv'
    # est_data_7000 = pd.read_csv(est_file_7000)

    
    for i in range(0,1):
        
        
        real_file1 = u'./'+args.k+'/MicroCU.csv'
        real_data1 = pd.read_csv(real_file1)
        real_file2 = u'./'+args.k+'/MicroCM.csv'
        real_data2 = pd.read_csv(real_file2)
        real_file3 = u'./'+args.k+'/SI_CU.csv'
        real_data3 = pd.read_csv(real_file3)
        real_file4 = u'./'+args.k+'/SI_CM.csv'
        real_data4 = pd.read_csv(real_file4)
        real_file5 = u'./'+args.k+'/ECM.csv'
        real_data5 = pd.read_csv(real_file5)
        # real_file1 = u'./ddSketch/res/'+args.k+'.csv'
        # real_data1 = pd.read_csv(real_file1)
        # est_file_8000 = u'./result/top-' + args.k + u'_est_8000.csv'
        # est_data_8000 = pd.read_csv(est_file_8000)

        # plt.rc('font', family='SimSun-ExtB')
        # plt.xlim(0,1000000000)
        # plt.xscale('symlog', linthresh=0.000000000000000000001)

        ax = plt.gca()

        plt.figure(figsize=(5.5, 4.5))
        plt.yscale('linear')
        plt.tick_params(labelsize = 13)
        # plt.rcParams['font.sans-serif']=['SimHei']
        # plt.rcParams['axes.unicode_minus']=False
        # print(matplotlib.matplotlib_fname())
        # print(matplotlib.get_cachedir())

        plt.xlabel(r'Memory (KB)',fontweight='bold',fontsize=24)#设置x轴，并设定字号大小
        plt.ylabel(u'AAE',fontweight='bold',fontsize=24)#设置y轴，并设定字号大小
        # ax.set_xscale('log')
        plt.yscale('log')
        # plt.xscale('symlog', linthresh=0.01)


        # ax = plt.gca()

        # if args.log == True:
        #     ax.set_xscale('log')
        # if args.k=="res_tiny":
        #     plt.plot(real_data['memory'], real_data['MAE'], label='tiny', linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        # elif args.k=="tiny_dist":
        
        plt.plot(real_data1['mem'], real_data1['aae'], label="Ours-CU", linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        plt.plot(real_data2['mem'], real_data2['aae'], label="Ours-CM", linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        plt.plot(real_data3['mem'], real_data3['aae'], label="SI-CU", linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        
        plt.plot(real_data4['mem'], real_data4['aae'], label="SI-CM", linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        plt.plot(real_data5['mem'], real_data5['aae'], label="ECM", linestyle='-', marker=None, markersize=10, alpha = 1, linewidth=2, markerfacecolor='none', markeredgewidth=2)
        
        
        plt.legend(loc = 'best', ncol=1, handlelength=3)
        leg = plt.gca().get_legend() #或leg=ax.get_legend()
        ltext = leg.get_texts()
        plt.setp(ltext,fontweight='bold',fontsize = 20)
        #plt.setp(ltext,fontsize = 22)

        #plt.legend(fontsize = 16)
        plt.grid(True, linestyle='--', axis='y')
        plt.grid(True, linestyle='--', axis='x')
        plt.tight_layout()

        plt.savefig('./'+args.k+'/'+args.k+'.pdf')
        plt.show()


if __name__ == "__main__":
    main()
