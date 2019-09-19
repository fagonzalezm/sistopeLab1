all: 
	@$(MAKE) -f main.mk
	@$(MAKE) -f convolution.mk
	@$(MAKE) -f rectification.mk
	@$(MAKE) -f pooling.mk
	@$(MAKE) -f classifier.mk
	@$(MAKE) -f resultsWriter.mk
	