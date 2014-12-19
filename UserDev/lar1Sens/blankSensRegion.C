

{

      lar1::PlotUtils plotUtils;
      TLegend * leg3 = plotUtils.getLSNDLegend();
      TCanvas * tempCanv = new TCanvas("tempCanv","temp canvas",650,650);
      TH2D * hr1 = plotUtils.getEmptySensPlot();
      tempCanv->cd();
      TPad * padTemp = new TPad("padTemp","padTemp",0,0,1,1);
      padTemp->Draw();
      padTemp->cd();
      padTemp->SetLogx();
      padTemp->SetLogy();
      padTemp -> Update();

      hr1->Draw();
      plotUtils.lsnd_plot(padTemp);
      // plotUtils.miniBoone_plot(padTemp);
      plotUtils.giunti_plot(padTemp);
      leg3->Draw("same");
}