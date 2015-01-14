# coding: utf-8


import xml.etree.cElementTree as ET
import nltk.data

tokenizer = nltk.data.load("tokenizers/punkt/english.pickle")

fp = open("anna.txt")

text = fp.read().decode('utf-8')
text = text.replace('\n', ' ').replace('\r', '')
text = text.replace(';', '.')
text = text.replace('_he_', 'he')
#text= text.replace("\xe2\x80\x99", "\'")

sent_tokenize_list = tokenizer.tokenize(text)

#print  sent_tokenize_list
#print len(sent_tokenize_list)

root = ET.Element("root")

count =0; 
for sent in sent_tokenize_list:
	firstWord = sent.split(' ', 1)[0]
	if (firstWord == 'She'):
		if (sent[len(sent)-1] == "\""):
			if (sent.count("\"")%2 == 1):
				sent =sent[:len(sent)-1]
		print sent
		shePhrase = ET.SubElement(root, "shePhrase")
		shePhrase.text = sent
		count+=1
		
print count

tree = ET.ElementTree(root)
tree.write("sheFile.xml")


