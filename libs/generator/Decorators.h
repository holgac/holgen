#pragma once


namespace holgen {
  // This kinda serves as documentation
  class Decorators {
  public:
    /*
     * Usage: Only with fields
     *
     * Example:
     *  @jsonConvert(from=string, using=function) // func(string) -> u8 argument added
     *  u8 country;
     *  @jsonConvert(from=string, using=function) // another func added
     *  u8 city;
     *  @jsonConvert(sameAs=country) // first func reused
     *  u8 previousCountry;
     *
     *  Normally
     */
    inline static const std::string JsonConvert = "jsonConvert";
    inline static const std::string JsonConvert_From = "from";
    inline static const std::string JsonConvert_Using = "using";
  };
}
