## Welcome to my Systems Engineering project!

### In this zip folder you should be able to find the following:

- main.cpp file containing my socket programming code in C++

- makefile that can be run to build the C++ code

- screenshots of this program's output for my Cloudflare Workers site as well as other sites

- this reamdme.md file (shocking, I know!)

### This CLI tool allows for the following arguements:

1.  `--help` OR `--url` (**required**)

    - Passing in `--help` as the first arguement will give a detailed explanation on how to use the tool
    - Passing in a `URL` as the first parameter (do not include `https://` , `http://` or `www.`) will execute an `HTTP GET` request to that domain/endpoint

2.  `--n` (**optional**)

- The second arguement takes an integer `n`, which will indicate the number of requests you will make to the given `URL`

### The CLI tool will return the full `HTTP` response, as well as metrics shown in the sample requests below:

`URL`: my-app.parsae.workers.dev

- 5 request(s)

- time 0 : 82700

- time 1 : 98262

- time 2 : 125287

- time 3 : 181899

- time 4 : 276873

- mean time: 153004

- median time: 125287

- minimum time: 82700

- maximum time: 276873

- successful responses: 100%

- unsuccessful responses: 0%

- largest response: 4304 bytes

- smallest response: 4208 bytes

`URL`: geeksforgeeks.com/c-plus-plus

- 5 request(s)

- time 0 : 164800

- time 1 : 167618

- time 2 : 168845

- time 3 : 170324

- time 4 : 230542

- mean time: 180426

- median time: 168845

- minimum time: 164800

- maximum time: 230542

- successful responses: 100%

- unsuccessful responses: 0%

- largest response: 4504 bytes

- smallest response: 4497 bytes


As you can see, geeksforgeeks.com/c-plus-plus and my workers sites both return around 4,000 to 4,500 byte responses. That being said, my workers page seems to perform better in response speed, beating the other side in mean, median, and minimum response time!
