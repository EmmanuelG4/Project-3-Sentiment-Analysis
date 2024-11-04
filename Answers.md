# Assignment 2: Answers

**Complete this document, commit your changes to Github and submit the repository URL to Canvas.** Keep your answers short and precise.

Your Name: Emmanuel Garcia

Used free extension: [ ] 24 hrs or [ ] 48 hrs

[ ] Early submission (48 hrs)

[ ] Bonus work. Describe: ...

Place [x] for what applies.


## UML Diagram

Add your UML class diagram here.

Replace the following image with your diagram. You can draw it by hand and take a photo.
![UML Class Diagram](/Project%203%20Class%20Diagram.png "Project 3 Class Diagram")

## Answers

1. How do you train the model and how do you classify a new tweet? Give a short description of the main steps.

    > The model is trained by tokenizing a tweet and associating the words of a tweet with positive emotion if the tweet had a sentiment value of 4, and negative emotion if the tweet had a sentiment value of 0. A tweet is classified by adding the positive and negative sentiment scores of all the words in a tweet and comparing the two scores after the program iterates through every word in the tweet. 

2. How long did your code take for training and what is the time complexity of your training implementation (Big-Oh notation)? Remember that training includes reading the tweets, breaking it into words, counting, ... Explain why you get this complexity (e.g., what does `N` stand for and how do your data structures/algorithms affect the complexity).

   > The code took about 0.26 seconds for training and the time complexity of the training implementation is O(N x log(W)), where N is the number of rows in a given file and W is the number of unique words. 

3. How long did your code take for classification and what is the time complexity of your classification implementation (Big-Oh notation)? Explain why.

   > The code took about 0.22 seconds for classification. The time complexity of the classification implementation is O(N x log(W)), where N is the number of tweets in a given file and W is the number of unique words.

4. What accuracy did your algorithm achieve on the provides training and test data? 

   > My accuracy: 50.02%

   The TA will run your code on Linux and that accuracy value will be used to determine your grade.

5. What were the changes that you made that improved the accuracy the most?
   
   > Downloading and implementing the CSV Parser Library by Vincent La improved the accuracy the most.  

6. How do you know that you use proper memory management? I.e., how do you know that you do not have
   a memory leak?

   > There is not a memory leak because a destructor for the DSString class was defined and implemented into several of the class's functions. Within the functions, the destructor is called to delete temporary and unecessary objects after they are no longer being used.

7. What was the most challenging part of the assignment?

   > The most challenging part of the assignment was parsing the information from the training datasets. 
