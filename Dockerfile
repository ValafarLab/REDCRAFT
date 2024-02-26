# Build from source using a build box
FROM ubuntu as build
# Set the Timezone
#Set the timezone
ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Update and Download dependancies
RUN apt-get update && apt-get install -y g++ cmake libomp-dev qt5-default libqt5charts5-dev
WORKDIR /redcraft
# Copy the Redcraft files into the build container
COPY . ./
# Run camke and make to build redcraft
RUN cmake . && make
# Create the application container
FROM ubuntu as app
#Set the timezone
ENV TZ=America/Chicago
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# Update and download dependancies
RUN apt-get update && apt-get install -y libgomp1 qt5-default libqt5charts5-dev vim
WORKDIR /redcraft
# Copy in the executables and scripts
COPY --from=build /redcraft/bin /usr/local/redcraft/bin/
COPY --from=build /redcraft/scripts /usr/local/redcraft/scripts/
COPY --from=build /redcraft/bin/redcraft /usr/local/bin/