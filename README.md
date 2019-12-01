# CokeDrumAI

## Introduction
Coke Drum AI was a project developed for the role out of the Avnet Azure Sphere Module Starter Kit on element14.com.

## Element14 Project Page:
https://www.element14.com/community/groups/azuresphere/blog/2019/10/17/avnet-azure-sphere-coke-drum-health-monitoring-iot-ai

## Hackster Project Page:
https://www.hackster.io/raisingawesome/avnet-azure-sphere-coke-drum-health-monitoring-iot-ai-d3f47b

## Makers and Students
So, if you are a maker who just wants to explore cloud based data storage and get an I2C, analog read, and Soft PWM project on the Azure Sphere.  Or if you are a student wanting help getting started on an assignment - you've come to the right place.

Copy down the code in the repository and then follow this blog to get your own Azure Sphere starter kit going:

https://www.element14.com/community/groups/azuresphere/blog/2019/04/24/avnets-azure-sphere-starter-kit-out-of-box-demo-part-1-of-3

To get the DarkSky.net working, you need to create your own account.  I've commented it out for now since you need to update your own "secrets"

Note:  per Microsoft, the ADC for analog input is still in Beta.  So, my code traps for its anomolies it presents.  You'll need to make some tweaks per the following page:

https://docs.microsoft.com/en-us/azure-sphere/app-development/use-beta

## Thoughts
Most of the magic happens in i2c.c.  I added the SoftPWM code and TFMini code there, too.  To understand all my tweaks to the original starter kit demo code, go to the initi2c function and starter scrolling down.  You'll see my comments pop in to give you ideas on how to add your own i2c devices.  Also, search out readDistance().  It's what talks to the TFMini directly within the i2c.c code.

-Sean J. Miller

raisingawesome@gmail.com
