
  # Read data:
  data_mkl_xeon       = read.table("mkl-xeon.txt",       col.names=c("Unknowns", "ExecTime", "GFLOPs"));
  data_mkl_phi        = read.table("mkl-phi.txt",        col.names=c("Unknowns", "ExecTime", "GFLOPs"));
  data_mkl_knl_ddr4   = read.table("mkl-knl-ddr4.txt",   col.names=c("Unknowns", "ExecTime", "GFLOPs"));
  data_mkl_knl_mcdram = read.table("mkl-knl-mcdram.txt", col.names=c("Unknowns", "ExecTime", "GFLOPs"));

  set1 <- c(1:9)  # matrices 1-9
  plotdata1 <- rbind(data_mkl_xeon$GFLOPs[set1], data_mkl_phi$GFLOPs[set1], data_mkl_knl_ddr4$GFLOPs[set1], data_mkl_knl_mcdram$GFLOPs[set1])
  set2 <- c(10:20)  # matrices 10-20
  plotdata2 <- rbind(data_mkl_xeon$GFLOPs[set2], data_mkl_phi$GFLOPs[set2], data_mkl_knl_ddr4$GFLOPs[set2], data_mkl_knl_mcdram$GFLOPs[set2])

  setEPS()
  postscript(paste("spgemm.eps", sep=""), width=14, height=9)

  legenditems = c("MKL 11.2.1, Xeon E5-2670 v3 x2 (Haswell)",
                  "MKL 11.2.1, Xeon Phi 7120 (KNC)",
                  "MKL 11.3.3, Xeon Phi 7250 (KNL, DDR4)",
                  "MKL 11.3.3, Xeon Phi 7250 (KNL, MCDRAM)"
                 )
  coloritems = c("darkblue", "steelblue1", "orange", "black")

  ## Plot at top:
  par(fig=c(0, 0.99, 0.45, 1.0))
  mp <- barplot(plotdata1, beside=TRUE, density=1, col=coloritems, ylim=c(0, 12), axes = FALSE)
  grid(NA, 3, col="black", lty=3)
  box(bty="c")
  box(lty=3)
  mp <- barplot(plotdata1, beside=TRUE, col=coloritems, ylim=c(0, 12), axes = FALSE, add=T)
  axis(4, at=c(0, 4, 8, 12), las=1, cex.axis=1.2)
  axis(2, at=c(0, 4, 8, 12), las=1, cex.axis=1.2)
  mtext(side = 2, "GFLOPs", line = 2.2, cex=1.2)
  text(5*(1:9)-1, par("usr")[3] - 0.75, srt = 30, adj = 1, labels = c("cantilever", "economics", "epidemiology", "harbor", "protein", "qcd", "ship", "spheres", "windtunnel"), xpd = TRUE)
  text(mp + 0.2, 10.9, format(round(plotdata1, 1), nsmall=1),cex=1.1,pos=3, col="black", srt=90, adj=1)

  ## Plot at bottom:
  par(fig=c(0, 0.99, 0.0, 0.55), new=TRUE)
  mp <- barplot(plotdata2, beside=TRUE, density=1, col=coloritems, ylim=c(0, 2), axes = FALSE)
  grid(NA, 4, col="black", lty=3)
  box(bty="c")
  box(lty=3)
  mp <- barplot(plotdata2, beside=TRUE, col=coloritems, ylim=c(0, 2), axes = FALSE, add=T)
  axis(4, at=c(0, 0.5, 1, 1.5, 2), las=1, cex.axis=1.2)
  axis(2, at=c(0, 0.5, 1, 1.5, 2), las=1, cex.axis=1.2)
  mtext(side = 2, "GFLOPs", line = 2.2, cex=1.2)
  text(5*(1:11)-1, par("usr")[3]- 0.15, srt = 30, adj = 1, labels = c("accelerator", "amazon0312", "ca-CondMat", "cit-Patents", "circuit", "email-Enron", "p2p-Gnutella31", "roadNet-CA", "webbase1m", "web-Google", "wiki-Vote"), xpd = TRUE)
  text(mp + 0.2, 1.8, format(round(plotdata2, 2), nsmall=1),cex=1.1,pos=3, col="black", srt=90, adj=1)

  # Legend:
  par(fig=c(0.0,1.0,0,1), new=TRUE, xpd=TRUE)
  legend(legend = legenditems[c(1:2)], x = 2, y = 2.5,
         text.width=c(23, 23),
         bty = "n", lwd=12, cex=1.1, horiz=T, col=coloritems[c(1:2)])
  legend(legend = legenditems[c(3:4)], x = 2, y = 2.4,
         text.width=c(23, 23),
         bty = "n", lwd=12, cex=1.1, horiz=T, col=coloritems[c(3:4)])

  dev.off()


